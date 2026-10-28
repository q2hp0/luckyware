#pragma once

#include <string>
#include <vector>

enum class act {
    none,
    watch,
    log,
    sus,
    kill
};

struct hit {
    unsigned long pid;
    std::string proc;
    std::string path;
    std::string reason;
    act a;
};
