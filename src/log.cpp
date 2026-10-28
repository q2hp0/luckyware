#include "log.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string t() {
    std::time_t x = std::time(nullptr);
    std::tm m;
#ifdef _WIN32
    localtime_s(&m, &x);
#else
    m = *std::localtime(&x);
#endif
    std::stringstream ss;
    ss << std::put_time(&m, "%H:%M:%S");
    return ss.str();
}

void lg(const std::string &s) { std::cout << "[" << t() << "] " << s << std::endl; }
void lw(const std::string &s) { std::cout << "[" << t() << "] ! " << s << std::endl; }
void la(const std::string &s) { std::cout << "[" << t() << "] * " << s << std::endl; }
void ld(const std::string &s) { std::cout << "[" << t() << "] . " << s << std::endl; }
