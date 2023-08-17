#include "lldb/API/SBDebugger.h"
using namespace lldb;

namespace lldb_analyze {
class ABSDebugger : public SBDebugger {
public:
  ABSDebugger();
  ABSDebugger(const lldb::SBDebugger &rhs);
  static ABSDebugger Create();
  static void Init();
};
} // namespace lldb_analyze
