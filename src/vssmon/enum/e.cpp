#include "vssmon/enum/e.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace vssen {
void start(){
    std::thread([](){ lg("vssen on"); while(true){ std::this_thread::sleep_for(std::chrono::seconds(5)); } }).detach();
}
}
