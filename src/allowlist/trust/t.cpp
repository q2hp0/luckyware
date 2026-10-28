#include "allowlist/trust/t.h"
#include "allowlist/allow.h"
#include "log.h"
namespace trust { bool ok(const char* p){ if(!p)return false; lg("trust chk"); return allow_ok(p); } }
