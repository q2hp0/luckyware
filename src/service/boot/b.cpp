#include "service/boot/b.h"
#include "log.h"
#include "engine/rules/r.h"
#include "canary/gen/g.h"
#include "fsmon/list/l.h"
#include "procguard/tree/t.h"
#include "handlemon/dup/d.h"
#include "debugmon/chk/c.h"
#include "vssmon/enum/e.h"
#include "allowlist/trust/t.h"
#include "responder/act/a.h"
#include "selfprotect/guard/g.h"
#include "etw/parse/p.h"

namespace boot {
void go(){
    rules::load();
    canaryg::make();
    fsl::init();
    pt::scan();
    hdup::start();
    dbgchk::start();
    vssen::start();
    spg::run();
    etwp::run();
    lg("boot done");
}
}
