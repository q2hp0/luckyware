#include "fsw.h"
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



void fsw_start() {
    std::thread([]() {
        lg("fsw on");
        while (true) {
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 pe;
                pe.dwSize = sizeof(pe);
                if (Process32First(snap, &pe)) {
                    do {
                        if (pe.th32ProcessID == GetCurrentProcessId()) continue;
                        std::string pp = ppath(pe.th32ProcessID);
                        if (pp.empty()) continue;
                        if (allow_ok(pp)) continue;
                        std::string lp = low(pp);
                        if (has(lp, "steal_")) {
                            hit h;
                            h.pid = pe.th32ProcessID;
                            h.proc = pp;
                            h.path = "steal-sim";
                            h.reason = "sim";
                            h.a = act::sus;
                            eng_push(h);
                            continue;
                        }
                    } while (Process32Next(snap, &pe));
                }
                CloseHandle(snap);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }).detach();
}
