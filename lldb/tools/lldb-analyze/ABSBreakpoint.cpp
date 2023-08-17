#include "ABSBreakpoint.h"
using namespace lldb_analyze;

ABSBreakpoint::ABSBreakpoint(const lldb::SBBreakpoint &rhs)
    : SBBreakpoint(rhs){};