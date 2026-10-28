#include "etw/parse/p.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace etwp {
void run(){
    std::thread([](){ lg("etwp on"); while(true){ std::this_thread::sleep_for(std::chrono::seconds(1)); } }).detach();
}
}
