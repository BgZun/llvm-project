#include "llvm/Transforms/Utils/DebugDelPass.h"
#include "llvm/IR/IntrinsicInst.h"


using namespace llvm;

PreservedAnalyses DebugDelPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  
  
    for (BasicBlock& b : F )
    {
        for (Instruction& i : make_early_inc_range(b))
        {
            if (i.isDebugOrPseudoInst())
            {
                if (DbgInfoIntrinsic::classof(&i))
                {
                    i.eraseFromParent();
                }
            }
            
        }
        
    }

    Module* m = F.getParent();

    for(Function& test : make_early_inc_range(*m)){
        if(test.isDeclaration() && test.getName().starts_with("llvm.dbg.") && test.getNumUses() == 0){
            
            test.eraseFromParent();
        }
    }

  return PreservedAnalyses::all();
}
