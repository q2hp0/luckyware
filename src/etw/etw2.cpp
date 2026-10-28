#include "etw/etw2.h"
#include "log.h"
#include <windows.h>
#include <thread>

void etw2_start()
{
    std::thread([]() {
        lg("etw init");
        while (true) {
            Sleep(5000);
        }
    }).detach();
}
