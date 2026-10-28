#include "procguard/tree/t.h"
#include "log.h"
#include "procguard/proc.h"
#include "allowlist/allow.h"
#include <windows.h>
#include <tlhelp32.h>
#include <thread>
#include <chrono>

namespace pt {
void scan(){
    std::thread([](){
        lg("pt scan");
        while(true){
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
            if(snap!=INVALID_HANDLE_VALUE){
                PROCESSENTRY32 pe; pe.dwSize=sizeof(pe);
                if(Process32First(snap,&pe)){
                    do{
                        std::string p = ppath(pe.th32ProcessID);
                        if(p.empty()) continue;
                        if(allow_ok(p)) continue;
                    }while(Process32Next(snap,&pe));
                }
                CloseHandle(snap);
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }).detach();
}
}
