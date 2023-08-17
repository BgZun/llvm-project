#include "ABSDebugger.h"
using namespace lldb;
using namespace lldb_analyze;

ABSDebugger::ABSDebugger() : SBDebugger(){};
ABSDebugger::ABSDebugger(const lldb::SBDebugger &rhs) : SBDebugger(rhs){};
void ABSDebugger::Init() { SBDebugger::Initialize(); }

ABSDebugger ABSDebugger::Create() { return SBDebugger::Create(); }