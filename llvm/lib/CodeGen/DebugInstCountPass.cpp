
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/InitializePasses.h"

using namespace llvm;
#define DEBUG_TYPE "mypass"

namespace llvm {
    class My : public MachineFunctionPass {
        
    public:
        static char ID;
        My() : MachineFunctionPass(ID){
            initializeMyPass(*PassRegistry::getPassRegistry());
        }
    private:
        
        bool runOnMachineFunction(MachineFunction &MF) override{
            int value = 0;
            int label = 0;
            int phi = 0;
            int ref = 0;
            if(skipFunction(MF.getFunction()))
                return false;

            for(MachineBasicBlock& b: MF ){
                for(MachineInstr& i: b){
                    if(i.isDebugValue()){
                         value++;
                    }
                    else if(i.isDebugLabel()){
                        label++;
                    }
                    else if(i.isDebugPHI()){
                        phi++;
                    }
                    else if(i.isDebugRef()){
                        ref++;
                    }
                }
            }
            
            outs()<< MF.getName()<< " : " <<'\n';
            outs()<<'\t'<<"DBG_VALUE: "<<value<<'\n';
            outs()<<'\t'<<"DBG_LABEL: "<<label<<'\n';
            outs()<<'\t'<<"DBG_PHI: "<<phi<<'\n';
            outs()<<'\t'<<"DBG_REF: "<<ref<<'\n';
            return true;  
        }
    };
    char My::ID = 0;
    char &MyID = My::ID;
}


INITIALIZE_PASS(My, DEBUG_TYPE, "DDD", false,false)