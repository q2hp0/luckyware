#include "engine/eng.h"
#include "log.h"
#include "allowlist/allow.h"
#include "procguard/proc.h"
#include "fsmon/fs.h"
#include "canary/canary_core.h"
#include "responder/rsp.h"
#include "str.h"
#include "ui/ui.h"
#include <queue>
#include <mutex>
#include <thread>
#include <map>
#include <chrono>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif

static std::queue<hit> q;
static std::mutex m;

void eng_init() {
    canary_init();
}

void eng_push(const hit &h) {
    std::lock_guard<std::mutex> lk(m);
    q.push(h);
}

void eng_run() {
    std::thread([](){
        lg("engine on");
        while (true) {
            hit h;
            {
                std::lock_guard<std::mutex> lk(m);
                if (q.empty()) { std::this_thread::sleep_for(std::chrono::milliseconds(2)); continue; }
                h = q.front(); q.pop();
            }
            if (h.pid == 0) continue;
            if (h.pid == (unsigned long)GetCurrentProcessId()) continue;
            std::string pp = h.proc.empty() ? ppath(h.pid) : h.proc;
            std::string lp = low(pp);
            if (allow_ok(pp)) {
                continue;
            }
            std::string way = h.reason.empty() ? "file" : h.reason;
            if (canary_check(h.path)) {
                la("canary hit");
                rsp_stop(h.pid);
                ui_push(h.pid, lp.c_str(), pp.c_str(), h.path.c_str(), way.c_str(), "canary");
                lg(std::to_string(h.pid) + " | " + lp + " | " + h.path);
                continue;
            }
            if (is_tgt(h.path)) {
                la("target hit");
                rsp_stop(h.pid);
                ui_push(h.pid, lp.c_str(), pp.c_str(), h.path.c_str(), way.c_str(), "target");
                lg(std::to_string(h.pid) + " | " + lp + " | " + h.path);
                continue;
            }
            if (way == "handle") {
                la("target hit");
                rsp_stop(h.pid);
                ui_push(h.pid, lp.c_str(), pp.c_str(), h.path.c_str(), way.c_str(), "handle");
                lg(std::to_string(h.pid) + " | " + lp + " | " + h.path);
                continue;
            }
            if (way == "sim") {
                la("proc hit");
                rsp_stop(h.pid);
                ui_push(h.pid, lp.c_str(), pp.c_str(), h.path.c_str(), way.c_str(), "proc");
                lg(std::to_string(h.pid) + " | " + lp + " | " + h.path);
                continue;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }).detach();
}
