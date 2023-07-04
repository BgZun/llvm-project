#include "llvm/Transforms/Utils/BasicDebugInstCounter.h"
#include "llvm/IR/IntrinsicInst.h"

using namespace llvm;

PreservedAnalyses BasicFunctionPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  
  int values = 0;
  int declare = 0;
  int label = 0;
  
    for (BasicBlock& b : F )
    {
        for (Instruction& i : b)
        {
            if (i.isDebugOrPseudoInst())
            {
                if (DbgDeclareInst::classof(&i))
                {
                    declare++;
                }
                else if (DbgValueInst::classof(&i))
                {
                    values++;
                }
                else if(DbgLabelInst::classof(&i)){
                    label++;
                }
                
            }
            
        }
        
    }
    
    errs() <<"Function: " << F.getName() << '\n';
    errs() <<'\t' << "llvm.dbg.value : " << values << '\n';
    errs() <<'\t' << "llvm.dbg.declare : "<< declare << '\n';
    errs() <<'\t' << "llvm.dbg.label : "<< label << '\n';

    
  return PreservedAnalyses::all();
}
