#include "ABSBreakpoint.h"
#include "lldb/API/SBTarget.h"

namespace lldb_analyze {
class ABSTarget : public lldb::SBTarget {
private:
public:
  ABSTarget(const lldb::SBTarget &rhs);
  ABSBreakpoint CreateBreakpoint(const char *name,
                                 const char *module_name = nullptr,
                                 bool internal = false);
};
} // namespace lldb_analyze
