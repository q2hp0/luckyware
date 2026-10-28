#include "dbg.h"
#include "log.h"
#include "engine/eng.h"
#include "procguard/proc.h"
#include "allowlist/allow.h"
#include "responder/rsp.h"
#include "str.h"
#include <thread>
#include <chrono>
#include <windows.h>
#include <tlhelp32.h>

void dbg_start() {
    std::thread([](){
        lg("dbg on");
        while (true) {
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 pe;
                pe.dwSize = sizeof(pe);
                if (Process32First(snap, &pe)) {
                    do {
                        std::string p = ppath(pe.th32ProcessID);
                        if (p.empty()) continue;
                        if (allow_ok(p)) continue;
                        std::string lp = low(p);
                        if (has(lp, "cmd.exe") || has(lp, "powershell.exe") || has(lp, "rundll32.exe") || has(lp, "regsvr32.exe") || has(lp, "mshta.exe") || has(lp, "wscript.exe") || has(lp, "cscript.exe")) {
                            lw("weird parent");
                        }
                    } while (Process32Next(snap, &pe));
                }
                CloseHandle(snap);
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }).detach();
}
