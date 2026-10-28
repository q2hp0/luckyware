#pragma once

#include <string>
#include <windows.h>

namespace hand {
    void start();
    bool is_opening(const std::string &proc, const std::string &path);
}
