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
#include "lldb/API/SBStream.h"
#include "lldb/API/SBSourceManager.h"
#include "lldb/API/SBExecutionContext.h"
#include "lldb/API/SBCommandReturnObject.h"
#include "lldb/API/SBCommandInterpreter.h"

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

  //Create a target from the provided exe file
  //Exit if the target isn't created
  lldb::SBTarget target = debugger.CreateTarget(TargetFileName.c_str());
  if(!target.IsValid()){
    return -1;
  }

  //Setup
  //Set target and a breakpoint for the main function(if it exists)
  debugger.SetSelectedTarget(target);

  target.BreakpointCreateByName("main");

  //Initial loop
  //We run this loop until the user quits or starts the process from the proces
  while (true)
  {
    string buff;
        getline(cin,buff);
        debugger.HandleCommand(buff.c_str());
        
        if(debugger.GetCommandInterpreter().HasCustomQuitExitCode()){
          return 0;
        }
        else if(buff=="q"){
          return 0;
        }
        else if(target.GetProcess().IsValid()){
          break;
        }
  }
  
  //Setup support vars for main loop
  lldb::SBProcess proc = target.GetProcess();
  lldb::SBEvent event;
  lldb::SBStream str_temp;
  lldb::SBListener lis = debugger.GetListener();
  str_temp.RedirectToFileHandle(stdout,false);

  //Main loop
  //We run this loop untill the running process exits
  //The process can be killed and replaced by the user but the loop terminates only when it exits
  while(true){
    //Get current process
    //Required since the user can use commands such as 'r' durring debugging to kill and start a new process
    proc = target.GetProcess();

    //Handle events
    while (lis.GetNextEvent(event))
    {
      debugger.HandleProcessEvent(proc,event,debugger.GetOutputFile(),debugger.GetErrorFile());
    }
    //Breakpoint hit
    if (proc.GetState()==lldb::eStateStopped){
      
        str_temp.Printf("Process %" PRIu64" stopped\n",proc.GetProcessID());

        //Get the current thread and selected frame
        lldb::SBThread cur_thread = proc.GetThreadAtIndex(0);
        lldb::SBFrame selected_frame = cur_thread.GetSelectedFrame();
        
        //Print the current thread and selected frame info
        cur_thread.GetDescription(str_temp);
        selected_frame.GetDescription(str_temp);
        
        //Get the SymbolContext for source line printing
        lldb::SBAddress addr = selected_frame.GetPCAddress();
        lldb::SBSymbolContext sy_cx = target.ResolveSymbolContextForAddress(addr,lldb::eSymbolContextEverything);
        
        //Are we stil in the source file ?
        if(selected_frame.GetLineEntry().IsValid()){
          //Print 3 lines of source code before and after the current line
          target.GetSourceManager().DisplaySourceLinesWithLineNumbersAndColumn(sy_cx.GetLineEntry().GetFileSpec(),sy_cx.GetLineEntry().GetLine(),sy_cx.GetLineEntry().GetColumn(),3,3,"->",str_temp);
        }
        else{
          //Switched to ASM

          //Print asm function
          //Print format similar to LLDB
          cout<<selected_frame.GetModule().GetFileSpec().GetFilename()<<"`"<<selected_frame.GetFunctionName()<<':'<<endl;
          
          //Offset in the function
          // cout<<" + "<<selected_frame.GetPCAddress().GetOffset()-selected_frame.GetModule().ResolveSymbolContextForAddress(selected_frame.GetPCAddress(),lldb::eSymbolContextEverything).GetSymbol().GetStartAddress().GetOffset();

          //Get asm
          string c = selected_frame.Disassemble();
          
          //Find the current inst
          size_t idx = c.find("->",0);
          //Include color
          idx -=5;

          //Print next 4 lines
          if(idx!=string::npos){
            size_t lines = 4;
            while (lines>0 && idx<c.size())
            {
              cout<<c.c_str()[idx];
              if(c[idx]=='\n'){
                lines--;
              }
              idx++;
            }
            cout<<endl;
          }
        
        }
        //Handle new command
        string buff;
        getline(cin,buff);
        
        //Special case 
        //If the input is the quit command we need to make sure the user acctualy quits
        //When the command is executed the user will be asked if the process should be killed
        //If the user chooses not to kill the process the loop continues
        if(buff=="q"){
          lldb::SBCommandReturnObject result;
          //Manualy handle the command via the SBCommandInterpeter in order to get the result
          debugger.GetCommandInterpreter().HandleCommand(buff.c_str(),result);
          

          //Did the user choose to kill the process ?
          if(result.GetStatus() & lldb::eReturnStatusQuit){
            break;
          }
        }
        else{
          debugger.HandleCommand(buff.c_str());
        }
        
        //Check if we have quit with or without and exitcode
        if(debugger.GetCommandInterpreter().GetQuitStatus()){
          break;
        }
      }
      else if(proc.GetState()==lldb::eStateExited){
        //If the process has exited print its status
        int exit_status = proc.GetExitStatus();
        const char* exit_desc = proc.GetExitDescription();
        printf("Process %" PRIu64 " exited with status = %i (0x%8.8x) %s\n",
                    proc.GetProcessID(), exit_status, exit_status,
                    exit_desc ? exit_desc : "");
        break;
        
      }
  }
    //If the user has quit, kill the process
    if(proc.IsValid()){
      proc.Kill();
    }
    return 0;
}