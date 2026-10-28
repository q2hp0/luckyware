#include "responder/rsp.h"
#include <windows.h>
#include <tlhelp32.h>

#ifndef NTSTATUS
typedef long NTSTATUS;
#endif

typedef NTSTATUS (NTAPI *pntsp)(HANDLE);
typedef NTSTATUS (NTAPI *pntres)(HANDLE);

void rsp_stop(unsigned long pid) {
    if (pid == 0) return;
    HANDLE h = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, (DWORD)pid);
    if (!h) h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
    if (!h) return;
    HMODULE ntd = GetModuleHandleA("ntdll.dll");
    if (ntd) {
        pntsp fn = (pntsp)GetProcAddress(ntd, "NtSuspendProcess");
        if (fn) fn(h);
    }
    CloseHandle(h);
}

void rsp_keep(unsigned long pid) {
    if (pid == 0) return;
    HMODULE ntd = GetModuleHandleA("ntdll.dll");
    if (!ntd) return;
    HANDLE h = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, (DWORD)pid);
    if (!h) return;
    pntres fn = (pntres)GetProcAddress(ntd, "NtResumeProcess");
    if (fn) fn(h);
    CloseHandle(h);
}

void rsp_hard(unsigned long pid) {
    if (pid == 0) return;
    HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)pid);
    if (!h) return;
    TerminateProcess(h, 9);
    CloseHandle(h);
}
