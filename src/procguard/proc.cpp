#include "proc.h"
#include "str.h"
#include <vector>
#include <algorithm>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <wintrust.h>
#include <softpub.h>
#include <winternl.h>

#pragma comment(lib, "wintrust.lib")
#pragma comment(lib, "crypt32.lib")

static bool is_int(const std::string &lowp) {
    if (has(lowp, "\\windows\\system32\\")) return true;
    if (has(lowp, "\\windows\\syswow64\\")) return true;
    if (has(lowp, "\\windows\\winsxs\\")) return true;
    return false;
}

std::string ppath(unsigned long pid) {
    if (pid == 0) return "";
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (!h) h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)pid);
    if (!h) return "";
    char buf[MAX_PATH * 2];
    DWORD sz = MAX_PATH * 2;
    if (QueryFullProcessImageNameA(h, 0, buf, &sz)) {
        CloseHandle(h);
        return std::string(buf);
    }
    CloseHandle(h);
    return "";
}

std::string ppar(unsigned long pid) {
    if (pid == 0) return "";
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return "";
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    std::string par;
    if (Process32First(snap, &pe)) {
        do {
            if (pe.th32ProcessID == (DWORD)pid) {
                par = ppath(pe.th32ParentProcessID);
                break;
            }
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
    return par;
}

std::string pcmd(unsigned long pid) {
    if (pid == 0) return "";
    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)pid);
    if (!h) h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (!h) return "";
    std::string res;
    CloseHandle(h);
    return res;
}

bool is_sys(const std::string &p) {
    std::string lp = low(p);
    return is_int(lp);
}

bool has_sig_ok(const std::string &p) {
    if (p.empty()) return false;
    WINTRUST_FILE_INFO fi = {0};
    fi.cbStruct = sizeof(WINTRUST_FILE_INFO);
    std::wstring wp = tow(p);
    fi.pcwszFilePath = wp.c_str();
    fi.hFile = NULL;
    fi.pgKnownSubject = NULL;

    GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA wd = {0};
    wd.cbStruct = sizeof(WINTRUST_DATA);
    wd.dwUnionChoice = WTD_CHOICE_FILE;
    wd.pFile = &fi;
    wd.dwUIChoice = WTD_UI_NONE;
    wd.fdwRevocationChecks = WTD_REVOKE_NONE;
    wd.dwStateAction = WTD_STATEACTION_VERIFY;
    wd.dwProvFlags = WTD_SAFER_FLAG;
    wd.dwUIContext = 0;
    LONG r = WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &action, &wd);
    wd.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust((HWND)INVALID_HANDLE_VALUE, &action, &wd);
    return r == ERROR_SUCCESS;
}
