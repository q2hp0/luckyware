#pragma once
void ui_run();
extern "C" void ui_push(unsigned long pid, const char* proc, const char* ppath, const char* file, const char* way, const char* reason);
