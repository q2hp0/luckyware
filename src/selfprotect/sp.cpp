#include "selfprotect/sp.h"
#include "log.h"
#include <thread>
#include <chrono>

namespace sp {
void start() {
    std::thread([](){
        lg("sp on");
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }).detach();
}
}
