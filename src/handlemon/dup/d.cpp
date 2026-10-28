#include "handlemon/dup/d.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace hdup {
void start(){
    std::thread([](){ lg("hdup on"); while(true){ std::this_thread::sleep_for(std::chrono::seconds(4)); } }).detach();
}
}
