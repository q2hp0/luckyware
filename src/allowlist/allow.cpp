#include "allow.h"
#include "str.h"
#include "proc.h"

bool allow_ok(const std::string &p) {
    if (p.empty()) return false;
    std::string lp = low(p);
    if (has(lp, "chrome.exe")) return true;
    if (has(lp, "msedge.exe")) return true;
    if (has(lp, "brave.exe")) return true;
    if (has(lp, "vivaldi.exe")) return true;
    if (has(lp, "opera.exe")) return true;
    if (has(lp, "operagx.exe")) return true;
    if (has(lp, "firefox.exe")) return true;
    if (has(lp, "waterfox.exe")) return true;
    if (has(lp, "librewolf.exe")) return true;
    if (has(lp, "discord.exe")) return true;
    if (has(lp, "discordcanary.exe")) return true;
    if (has(lp, "discordptb.exe")) return true;
    if (has(lp, "telegram.exe")) return true;
    if (has(lp, "whatsapp.exe")) return true;
    if (is_sys(lp)) {
        if (has_sig_ok(p)) return true;
    }
    if (has_sig_ok(p)) {
        if (has(lp, "\\program files\\")) return true;
        if (has(lp, "\\program files (x86)\\")) return true;
    }
    return false;
}
