#include "str.h"
#include <algorithm>
#include <cctype>
#ifdef _WIN32
#include <windows.h>
#endif

std::string low(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
    return s;
}

bool has(const std::string &s, const std::string &f) {
    if (s.empty() || f.empty()) return false;
    return s.find(f) != std::string::npos;
}

std::wstring tow(const std::string &s) {
#ifdef _WIN32
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
    if (n <= 0) return L"";
    std::wstring w(n, 0);
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, &w[0], n);
    return w;
#else
    (void)s;
    return L"";
#endif
}

std::string fromw(const std::wstring &w) {
#ifdef _WIN32
    if (w.empty()) return "";
    int n = WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, NULL, 0, NULL, NULL);
    if (n <= 0) return "";
    std::string s(n, 0);
    WideCharToMultiByte(CP_ACP, 0, w.c_str(), -1, &s[0], n, NULL, NULL);
    return s;
#else
    (void)w;
    return "";
#endif
}
