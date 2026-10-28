#pragma once

#include <string>

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

void eng_init();
void eng_push(const hit &h);
void eng_run();
