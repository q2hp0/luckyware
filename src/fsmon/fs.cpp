#include "fs.h"
#include "str.h"
#include "canary_core.h"

bool is_tgt(const std::string &p) {
    if (p.empty()) return false;
    std::string lp = low(p);
    if (canary_check(p)) return true;
    if (has(lp, "\\google\\chrome\\user data\\")) {
        if (has(lp, "\\network\\cookies")) return true;
        if (has(lp, "login data")) return true;
        if (has(lp, "web data")) return true;
        if (has(lp, "local state")) return true;
    }
    if (has(lp, "\\microsoft\\edge\\user data\\")) {
        if (has(lp, "\\network\\cookies")) return true;
        if (has(lp, "login data")) return true;
        if (has(lp, "web data")) return true;
        if (has(lp, "local state")) return true;
    }
    if (has(lp, "\\brave-software\\brave-browser\\user data\\")) {
        if (has(lp, "\\network\\cookies")) return true;
        if (has(lp, "login data")) return true;
        if (has(lp, "web data")) return true;
        if (has(lp, "local state")) return true;
    }
    if (has(lp, "\\opera software\\opera stable\\")) {
        if (has(lp, "\\network\\cookies")) return true;
        if (has(lp, "login data")) return true;
    }
    if (has(lp, "\\vivaldi\\user data\\")) {
        if (has(lp, "\\network\\cookies")) return true;
        if (has(lp, "login data")) return true;
    }
    if (has(lp, "\\mozilla\\firefox\\profiles\\")) {
        if (has(lp, "cookies.sqlite")) return true;
        if (has(lp, "logins.json")) return true;
        if (has(lp, "key4.db")) return true;
        if (has(lp, "cert9.db")) return true;
    }
    if (has(lp, "\\appdata\\roaming\\discord\\")) {
        if (has(lp, "\\local storage\\leveldb\\")) return true;
        if (has(lp, "\\session storage\\")) return true;
    }
    if (has(lp, "\\appdata\\roaming\\discordcanary\\")) {
        if (has(lp, "\\local storage\\leveldb\\")) return true;
    }
    if (has(lp, "\\appdata\\roaming\\discordptb\\")) {
        if (has(lp, "\\local storage\\leveldb\\")) return true;
    }
    if (has(lp, "\\appdata\\local\\google\\chrome\\user data\\")) return false;
    return false;
}
