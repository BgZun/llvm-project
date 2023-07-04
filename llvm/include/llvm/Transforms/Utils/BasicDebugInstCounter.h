#ifndef LLVM_TRANSFORMS_BASIC_FUNCTION_PASS_H
#define LLVM_TRANSFORMS_BASIC_FUNCTION_PASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class BasicFunctionPass : public PassInfoMixin<BasicFunctionPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

}

#endif 