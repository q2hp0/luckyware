#include "selfprotect/guard/g.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace spg {
void run(){
    std::thread([](){ lg("spg on"); while(true){ std::this_thread::sleep_for(std::chrono::seconds(6)); } }).detach();
}
}
