#include "canary/gen/g.h"
#include "log.h"
#include <string>
#include <fstream>
#include <windows.h>
#include <shlobj.h>

static std::string ad(){ char b[MAX_PATH]; if(SUCCEEDED(SHGetFolderPathA(NULL,CSIDL_APPDATA,NULL,0,b)))return std::string(b); return ""; }
static std::string ld(){ char b[MAX_PATH]; if(SUCCEEDED(SHGetFolderPathA(NULL,CSIDL_LOCAL_APPDATA,NULL,0,b)))return std::string(b); return ""; }

namespace canaryg {
void make(){
    std::string a=ad(), l=ld();
    if(!l.empty()){
        std::ofstream(l+"\\google\\chrome\\user data\\default\\network\\tmp_c").close();
        std::ofstream(l+"\\microsoft\\edge\\user data\\default\\network\\tmp_c").close();
        std::ofstream(l+"\\brave-software\\brave-browser\\user data\\default\\network\\tmp_c").close();
        std::ofstream(l+"\\opera software\\opera stable\\network\\tmp_c").close();
        std::ofstream(l+"\\vivaldi\\user data\\default\\network\\tmp_c").close();
    }
    if(!a.empty()){
        std::ofstream(a+"\\discord\\local storage\\leveldb\\tmp_d").close();
        std::ofstream(a+"\\discordcanary\\local storage\\leveldb\\tmp_d").close();
        std::ofstream(a+"\\discordptb\\local storage\\leveldb\\tmp_d").close();
    }
    if(!l.empty()){
        std::ofstream(l+"\\mozilla\\firefox\\profiles\\tmp_f").close();
    }
    lg("canary gen");
}
}
