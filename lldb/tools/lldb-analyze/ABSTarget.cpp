#include "ABSTarget.h"
#include "lldb/Target/Target.h"
#include "lldb/Utility/FileSpecList.h"
using namespace lldb_analyze;

ABSTarget::ABSTarget(const lldb::SBTarget &rhs) : SBTarget(rhs){};

ABSBreakpoint ABSTarget::CreateBreakpoint(const char *name,
                                          const char *module_name,
                                          bool internal) {

  // New implementation of SBTarget::BreakpointCreateByName
  lldb::TargetSP target_sp(GetSP());
  lldb::SBBreakpoint empty;
  if (target_sp.get()) {
    std::lock_guard<std::recursive_mutex> guard(target_sp->GetAPIMutex());

    const bool hardware = false;
    const lldb_private::LazyBool skip_prologue =
        lldb_private::eLazyBoolCalculate;
    const lldb::addr_t offset = 0;
    lldb::BreakpointSP br_sp;
    if (module_name && module_name[0]) {
      lldb_private::FileSpecList module_spec_list;
      module_spec_list.Append(lldb_private::FileSpec(module_name));
      br_sp = target_sp->CreateBreakpoint(&module_spec_list, nullptr, name,
                                          lldb::eFunctionNameTypeAuto,
                                          lldb::eLanguageTypeUnknown, offset,
                                          skip_prologue, internal, hardware);
    } else {
      br_sp = target_sp->CreateBreakpoint(nullptr, nullptr, name,
                                          lldb::eFunctionNameTypeAuto,
                                          lldb::eLanguageTypeUnknown, offset,
                                          skip_prologue, internal, hardware);
    }

    // Workaround for creating an SBBreakpoint since the
    // conversion constructor from BreakpointSP to SBBreakpoint is private
    br_sp->SetBreakpointKind("Internal break");
    return this->FindBreakpointByID(br_sp->GetID());
  }
  return empty;
}