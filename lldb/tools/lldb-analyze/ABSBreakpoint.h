#include "lldb/API/SBBreakpoint.h"
namespace lldb_analyze {
class ABSBreakpoint : public lldb::SBBreakpoint {
private:
public:
  ABSBreakpoint(const lldb::SBBreakpoint &rhs);
};
} // namespace lldb_analyze
