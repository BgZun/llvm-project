#include <iostream>

using namespace std;

#include "llvm/Support/CommandLine.h"

#include "lldb/API/SBDebugger.h"
#include "lldb/API/SBTarget.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBLaunchInfo.h"
#include "lldb/API/SBListener.h"
#include "lldb/API/SBEvent.h"
#include "lldb/API/SBFile.h" 
#include "lldb/API/SBFrame.h"
#include "lldb/API/SBThread.h"

using namespace llvm;

/// @}
/// Command line options.
/// @{
namespace {
using namespace cl;

static opt<std::string> TargetFileName("target-file", cl::Positional,
                                     cl::desc("<path to file>"),
                                     cl::Required);

} // namespace
/// @}
//===----------------------------------------------------------------------===//


int main(int argc, char **argv) {
  cl::ParseCommandLineOptions(argc, argv, "temp\n");

  llvm::outs() << "INPUT: " << TargetFileName.c_str()<<'\n';

  lldb::SBDebugger debugger;
  lldb::SBDebugger::Initialize();
  debugger = lldb::SBDebugger::Create();

  debugger.SetErrorFileHandle(stderr, false);
  debugger.SetOutputFileHandle(stdout,false);
  debugger.SetInputFileHandle(stdin,true);


  std::string s1 = TargetFileName.c_str();
  std::string s = Twine("file " + s1).str();

  lldb::SBTarget target = debugger.CreateTarget(TargetFileName.c_str());
  debugger.SetSelectedTarget(target);
  lldb::SBLaunchInfo li = target.GetLaunchInfo();

  lldb::SBError err;

  lldb::SBListener lis = debugger.GetListener();

  li.SetListener(lis);
  target.BreakpointCreateByName("main");
  target.SetLaunchInfo(li);

  lldb::SBProcess proc= target.Launch(li,err);

  lldb::SBEvent event;
  
  lldb::SBError err2;
  while(proc.GetExitStatus()==-1){
    while (lis.GetNextEvent(event))
    {
      // cout<<event.GetType()<<endl;
      debugger.HandleProcessEvent(proc,event,debugger.GetOutputFile(),debugger.GetErrorFile());
    }
    if (proc.GetState()==lldb::eStateStopped){
      lldb::SBAddress addr = proc.GetThreadAtIndex(0).GetSelectedFrame().GetPCAddress();
      lldb::SBSymbolContext sy_cx = proc.GetTarget().ResolveSymbolContextForAddress(addr,lldb::eSymbolContextEverything);
      cout<<"Cur Line:"<<sy_cx.GetLineEntry().GetLine()<<endl;
      proc.Continue();
    }
  }
    return 0;
}