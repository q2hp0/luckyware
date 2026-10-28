#include "handlemon/hand.h"
#include "log.h"
#include "procguard/proc.h"
#include "allowlist/allow.h"
#include "str.h"
#include <thread>
#include <chrono>
#include <vector>
#include <tlhelp32.h>

namespace hand {



void start() {
    std::thread([](){
        lg("hand on");
        while (true) {
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 pe;
                pe.dwSize = sizeof(pe);
                if (Process32First(snap, &pe)) {
                    do {
                        std::string pp = ppath(pe.th32ProcessID);
                        if (pp.empty()) continue;
                        if (allow_ok(pp)) continue;
                    } while (Process32Next(snap, &pe));
                }
                CloseHandle(snap);
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }).detach();
}

bool is_opening(const std::string &proc, const std::string &path) {
    (void)proc; (void)path;
    return false;
}
}
