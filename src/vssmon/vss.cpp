#include "vssmon/vss.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace vss {
void start() {
    std::thread([](){
        lg("vss on");
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(4));
        }
    }).detach();
}
}
