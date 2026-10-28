#include "ui/ui.h"
#include "engine/eng.h"
#include "log.h"
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <tlhelp32.h>

struct ev {
    unsigned long pid;
    unsigned long ppid;
    std::string proc;
    std::string ppath;
    std::string cmd;
    std::string file;
    std::string way;
    std::string reason;
    std::string time;
};

static HWND hwnd = NULL;
static HWND list = NULL;
static HWND det = NULL;
static std::vector<ev> items;
static std::map<int,int> idx;

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_CREATE) {
        list = CreateWindowExA(0, "LISTBOX", "", WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_VSCROLL|WS_BORDER, 2,2,596,200, h, (HMENU)1, NULL, NULL);
        det  = CreateWindowExA(0, "EDIT", "", WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE|ES_READONLY|WS_BORDER, 2,206,596,188, h, (HMENU)2, NULL, NULL);
        return 0;
    }
    if (m == WM_COMMAND) {
        if (LOWORD(w) == 1 && HIWORD(w) == LBN_SELCHANGE) {
            int sel = (int)SendMessageA(list, LB_GETCURSEL, 0, 0);
            if (sel >= 0 && idx.find(sel) != idx.end()) {
                int id = idx[sel];
                if (id >= 0 && id < (int)items.size()) {
                    ev &e = items[id];
                    std::string s;
                    s += "pid: " + std::to_string(e.pid) + "\r\n";
                    s += "ppid: " + std::to_string(e.ppid) + "\r\n";
                    s += "proc: " + e.proc + "\r\n";
                    s += "path: " + e.ppath + "\r\n";
                    s += "cmd: " + e.cmd + "\r\n";
                    s += "file: " + e.file + "\r\n";
                    s += "way: " + e.way + "\r\n";
                    s += "reason: " + e.reason + "\r\n";
                    s += "time: " + e.time + "\r\n";
                    SetWindowTextA(det, s.c_str());
                }
            }
        }
    }
    if (m == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(h,m,w,l);
}

void ui_add(const ev &e) {
    int id = (int)items.size();
    items.push_back(e);
    std::string row = std::to_string(e.pid) + " | " + e.proc;
    int pos = (int)SendMessageA(list, LB_ADDSTRING, 0, (LPARAM)row.c_str());
    idx[pos] = id;
}

void ui_run() {
    std::thread([]() {
        HINSTANCE h = GetModuleHandle(NULL);
        WNDCLASSA wc = {0};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = h;
        wc.lpszClassName = "alwu";
        wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
        RegisterClassA(&wc);
        hwnd = CreateWindowExA(0, "alwu", "", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 100,100,620,430, NULL, NULL, h, NULL);
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        MSG msg;
        while (GetMessageA(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }).detach();
}

static ev mk(unsigned long pid, const std::string &proc, const std::string &ppath, const std::string &file, const std::string &way, const std::string &reason) {
    ev e;
    e.pid = pid;
    e.ppath = ppath;
    e.proc = proc;
    e.file = file;
    e.way = way;
    e.reason = reason;
    e.ppid = 0;
    e.cmd = "";
    SYSTEMTIME st; GetLocalTime(&st);
    char buf[64]; sprintf(buf, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    e.time = buf;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe; pe.dwSize = sizeof(pe);
        if (Process32First(snap, &pe)) {
            do {
                if (pe.th32ProcessID == (DWORD)pid) {
                    e.ppid = pe.th32ParentProcessID;
                    break;
                }
            } while (Process32Next(snap, &pe));
        }
        CloseHandle(snap);
    }
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (h) {
        char c[1024]; DWORD sz = 1024;
        if (QueryFullProcessImageNameA(h, 0, c, &sz)) {}
        CloseHandle(h);
    }
    return e;
}

extern "C" void ui_push(unsigned long pid, const char* proc, const char* ppath, const char* file, const char* way, const char* reason) {
    ui_add(mk(pid, proc?proc:"", ppath?ppath:"", file?file:"", way?way:"", reason?reason:""));
}
