#include "mon_open.h"
#include "engine/eng.h"
#include "log.h"
#include "procguard/proc.h"
#include "allowlist/allow.h"
#include "canary/canary_core.h"
#include "fsmon/fs.h"
#include "str.h"
#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <tlhelp32.h>
#include <winternl.h>

typedef NTSTATUS (NTAPI *pNtQueryObject_t)(HANDLE, ULONG, PVOID, ULONG, PULONG);
typedef NTSTATUS (NTAPI *pNtQuerySystemInformation)(ULONG, PVOID, ULONG, PULONG);

#define SystemHandleInformation 16
#define ObjectNameInformation 1

typedef struct _SYSTEM_HANDLE {
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG HandleCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

static bool tgtfile(const std::string &fn) {
    if (fn.empty()) return false;
    std::string lp = low(fn);
    if (canary_check(fn)) return true;
    if (is_tgt(fn)) return true;
    if (has(lp, "cookies")) return true;
    if (has(lp, "login data")) return true;
    if (has(lp, "local state")) return true;
    if (has(lp, "web data")) return true;
    if (has(lp, "cookies.sqlite")) return true;
    if (has(lp, "logins.json")) return true;
    if (has(lp, "key4.db")) return true;
    if (has(lp, "leveldb")) return true;
    if (has(lp, "network\\cookies")) return true;
    return false;
}

static std::string getname(HANDLE h) {
    HMODULE ntd = GetModuleHandleA("ntdll.dll");
    pNtQueryObject_t qobj = (pNtQueryObject_t)GetProcAddress(ntd, "NtQueryObject");
    if (!qobj) return "";
    ULONG len = 0;
    if (qobj(h, (ULONG)ObjectNameInformation, NULL, 0, &len) != 0) return "";
    if (len == 0) return "";
    std::vector<BYTE> buf(len);
    if (qobj(h, (ULONG)ObjectNameInformation, buf.data(), len, &len) != 0) return "";
    PUNICODE_STRING us = (PUNICODE_STRING)buf.data();
    if (!us->Buffer) return "";
    return fromw(std::wstring(us->Buffer, us->Length / 2));
}

void mon_open_start() {
    std::thread([]() {
        lg("mon_open on");
        HMODULE ntd = GetModuleHandleA("ntdll.dll");
        pNtQuerySystemInformation qsi = (pNtQuerySystemInformation)GetProcAddress(ntd, "NtQuerySystemInformation");
        while (true) {
            if (!qsi) { Sleep(500); continue; }
            ULONG need = 0;
            qsi(SystemHandleInformation, NULL, 0, &need);
            if (need == 0) { Sleep(250); continue; }
            std::vector<BYTE> buf(need * 4);
            if (qsi(SystemHandleInformation, buf.data(), (ULONG)buf.size(), &need) != 0) { Sleep(250); continue; }
            PSYSTEM_HANDLE_INFORMATION shi = (PSYSTEM_HANDLE_INFORMATION)buf.data();
            for (ULONG i = 0; i < shi->HandleCount; ++i) {
                DWORD pid = shi->Handles[i].ProcessId;
                if (pid == GetCurrentProcessId()) continue;
                std::string pp = ppath(pid);
                if (pp.empty()) continue;
                if (allow_ok(pp)) continue;
                HANDLE ph = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
                if (!ph) ph = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pid);
                if (!ph) continue;
                HANDLE duph = NULL;
                if (DuplicateHandle(ph, (HANDLE)(ULONG_PTR)shi->Handles[i].Handle, GetCurrentProcess(), &duph, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
                    std::string nm = getname(duph);
                    if (tgtfile(nm)) {
                        la("target hit");
                        hit h;
                        h.pid = pid;
                        h.proc = pp;
                        h.path = nm;
                        h.reason = "handle";
                        h.a = act::sus;
                        eng_push(h);
                    }
                    CloseHandle(duph);
                }
                CloseHandle(ph);
            }
            Sleep(100);
        }
    }).detach();
}
