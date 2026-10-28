#include "debugmon/chk/c.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace dbgchk {
void start(){
    std::thread([](){ lg("dbgchk on"); while(true){ std::this_thread::sleep_for(std::chrono::seconds(3)); } }).detach();
}
}
