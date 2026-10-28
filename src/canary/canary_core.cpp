#include "canary_core.h"
#include "str.h"
#include <vector>
#include <fstream>
#include <windows.h>
#include <shlobj.h>

static std::vector<std::string> lst;

static std::string ad() {
    char b[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, b))) return std::string(b);
    return "";
}

static std::string ld() {
    char b[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, b))) return std::string(b);
    return "";
}

void canary_init() {
    lst.clear();
    std::string a = ad();
    std::string l = ld();
    if (!l.empty()) {
        lst.push_back(low(l + "\\google\\chrome\\user data\\default\\network\\xk_cookie"));
        lst.push_back(low(l + "\\google\\chrome\\user data\\default\\login data.bk"));
        lst.push_back(low(l + "\\microsoft\\edge\\user data\\default\\network\\xk_cookie"));
        lst.push_back(low(l + "\\brave-software\\brave-browser\\user data\\default\\network\\xk_cookie"));
        lst.push_back(low(l + "\\opera software\\opera stable\\network\\xk_cookie"));
        lst.push_back(low(l + "\\vivaldi\\user data\\default\\network\\xk_cookie"));
    }
    if (!a.empty()) {
        lst.push_back(low(a + "\\discord\\local storage\\leveldb\\dsc_tok"));
        lst.push_back(low(a + "\\discordcanary\\local storage\\leveldb\\dsc_tok"));
        lst.push_back(low(a + "\\discordptb\\local storage\\leveldb\\dsc_tok"));
    }
    if (!l.empty()) {
        std::string ff = l + "\\mozilla\\firefox\\profiles";
        lst.push_back(low(ff + "\\xk_c.sqlite"));
        lst.push_back(low(ff + "\\xk_l.json"));
        lst.push_back(low(ff + "\\xk_k.db"));
    }
    for (auto &x : lst) {
        std::ofstream f(x);
        if (f) { f << "z"; f.close(); }
    }
}

bool canary_check(const std::string &p) {
    if (p.empty()) return false;
    std::string lp = low(p);
    for (auto &c : lst) {
        if (lp == c) return true;
    }
    if (has(lp, "xk_cookie")) return true;
    if (has(lp, "xk_c.sqlite")) return true;
    if (has(lp, "xk_l.json")) return true;
    if (has(lp, "xk_k.db")) return true;
    if (has(lp, "dsc_tok")) return true;
    return false;
}
