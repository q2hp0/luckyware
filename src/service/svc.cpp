#include "service/svc.h"
#include "engine/eng.h"
#include "etw/etw2.h"
#include "canary/canary_core.h"
#include "debugmon/dbg.h"
#include "service/boot/b.h"
#include "log.h"
#include "fsw.h"
#include "mon_open.h"
#include "ui/ui.h"
#include <windows.h>

void svc_run() {
    ui_run();
    SetConsoleTitleA("anti-luckyware");
    lg("anti-luckyware starting");
    boot::go();
    canary_init();
    eng_init();
    eng_run();
    etw2_start();
    dbg_start();
    fsw_start();
    mon_open_start();
    lg("layers up");
    while (true) {
        Sleep(1000);
    }
}
