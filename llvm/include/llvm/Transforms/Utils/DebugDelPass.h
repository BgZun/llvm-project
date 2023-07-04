#ifndef LLVM_TRANSFORMS_DEBUG_DEL_PASS_H
#define LLVM_TRANSFORMS_DEBUG_DEL_PASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class DebugDelPass : public PassInfoMixin<DebugDelPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

}

#endif 