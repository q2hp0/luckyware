#pragma once

#include <string>

std::string ppath(unsigned long pid);
std::string pcmd(unsigned long pid);
std::string ppar(unsigned long pid);
bool is_sys(const std::string &p);
bool has_sig_ok(const std::string &p);
