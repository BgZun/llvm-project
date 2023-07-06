
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/InitializePasses.h"

using namespace llvm;
#define DEBUG_TYPE "mydel"

namespace llvm {
    class MyDEL : public MachineFunctionPass {
        
    public:
        static char ID;
        MyDEL() : MachineFunctionPass(ID){
            initializeMyPass(*PassRegistry::getPassRegistry());
        }
    private:
        
        bool runOnMachineFunction(MachineFunction &MF) override{
            if(skipFunction(MF.getFunction()))
                return false;
            
            for(MachineBasicBlock& b : MF){
                for(MachineInstr& i : make_early_inc_range(b)){
                    if(i.isDebugInstr()){
                        i.eraseFromParent();
                    }
                }
            }

            return true;  
        }
    };
    char MyDEL::ID = 0;
    char &MyDELID = MyDEL::ID;
}


INITIALIZE_PASS(MyDEL, DEBUG_TYPE, "DDD", false,false)