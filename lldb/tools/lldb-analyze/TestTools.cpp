#include <iostream>

using namespace std;

#include "llvm/Support/CommandLine.h"

#include "ABSDebugger.h"
#include "ABSTarget.h"
#include "lldb/API/SBCommandInterpreter.h"
#include "lldb/API/SBCommandReturnObject.h"
#include "lldb/API/SBDebugger.h"
#include "lldb/API/SBEvent.h"
#include "lldb/API/SBExecutionContext.h"
#include "lldb/API/SBFile.h"
#include "lldb/API/SBFrame.h"
#include "lldb/API/SBLaunchInfo.h"
#include "lldb/API/SBListener.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBSourceManager.h"
#include "lldb/API/SBStream.h"
#include "lldb/API/SBTarget.h"
#include "lldb/API/SBThread.h"
#include "lldb/Utility/ConstString.h"
#include <list>

using namespace llvm;
using namespace lldb;
using namespace lldb_analyze;

/// @}
/// Command line options.
/// @{
namespace {
using namespace cl;

static opt<std::string> TargetFileName("target-file", cl::Positional,
                                       cl::desc("<path to file>"),
                                       cl::Required);
static opt<bool> Verbose("verbose", cl::desc("Print output from lldb"));

static opt<bool> Async("async", cl::desc("Turn on debugger in async mode"));

} // namespace
/// @}
//===----------------------------------------------------------------------===//

void PrintSource(lldb::SBProcess proc, lldb::SBStream &str_out) {
  uint32_t cur_stop_id = proc.GetStopID();
  lldb::SBEvent ev = proc.GetStopEventForStopID(cur_stop_id);
  lldb::SBTarget target = proc.GetTarget();

  std::list<uint32_t> thread_idx;

  lldb::SBThread cur_thread;
  for (size_t i = 0; i < proc.GetNumThreads(); i++) {
    // proc.GetSelectedThread() does not always get the thread that is
    // stopped so we need to check that manualy It is also possible that
    // multiple threads will stop at the same breakpoint at the same time
    // The same behaviour is exhibited in lldb so we need to handle it
    // here
    lldb::SBThread test_thread = proc.GetThreadAtIndex(i);
    if (test_thread.GetStopReason() != lldb::eStopReasonNone) {
      thread_idx.push_back(i);
    }
  }

  // Print the source code for every stopped thread
  for (auto idx : thread_idx) {
    // Get the thread and the selected frame
    lldb::SBThread cur_thread = proc.GetThreadAtIndex(idx);
    lldb::SBFrame selected_frame = cur_thread.GetSelectedFrame();

    // Print the current thread and selected frame info
    cur_thread.GetDescription(str_out);
    selected_frame.GetDescription(str_out);

    // Get the SymbolContext for source line printing
    lldb::SBAddress addr = selected_frame.GetPCAddress();
    lldb::SBSymbolContext sy_cx = target.ResolveSymbolContextForAddress(
        addr, lldb::eSymbolContextEverything);

    // Are we stil in the source file ?
    if (selected_frame.GetLineEntry().IsValid()) {
      // Print 3 lines of source code before and after the current line
      target.GetSourceManager().DisplaySourceLinesWithLineNumbersAndColumn(
          sy_cx.GetLineEntry().GetFileSpec(), sy_cx.GetLineEntry().GetLine(),
          sy_cx.GetLineEntry().GetColumn(), 3, 3, "->", str_out);
    } else {
      // Switched to ASM

      // Print asm function
      // Print format similar to LLDB
      llvm::outs() << selected_frame.GetModule().GetFileSpec().GetFilename()
                   << "`" << selected_frame.GetFunctionName() << ':' << '\n';

      // Get asm
      string c = selected_frame.Disassemble();

      // Find the current inst
      size_t idx = c.find("->", 0);
      // Include color
      idx -= 5;

      // Print next 4 lines
      if (idx != string::npos) {
        size_t lines = 4;
        while (lines > 0 && idx < c.size()) {
          llvm::outs() << c.c_str()[idx];
          if (c[idx] == '\n') {
            lines--;
          }
          idx++;
        }
        llvm::outs() << '\n';
      }
    }
  }
}

int main(int argc, char **argv) {

  cl::ParseCommandLineOptions(argc, argv, "lldb-analyze\n");

  llvm::outs() << "INPUT: " << TargetFileName.c_str() << '\n';

  ABSDebugger debugger;
  ABSDebugger::Init();
  debugger = ABSDebugger::Create();

  debugger.SetErrorFileHandle(stderr, false);
  debugger.SetOutputFileHandle(stdout, false);
  debugger.SetInputFileHandle(stdin, true);

  debugger.SetAsync(Async);

  // Create a target from the provided exe file
  // Exit if the target isn't created
  ABSTarget target = debugger.CreateTarget(TargetFileName.c_str());
  if (!target.IsValid()) {
    return -1;
  }

  // Setup
  // Set target and a breakpoint for the main function(if it exists)
  debugger.SetSelectedTarget(target);

  // Create a breakpoint at the start of the main function
  target.CreateBreakpoint("main", nullptr, true);

  // Setup support vars for main loop
  uint32_t last_proc_stop_id = 0;
  uint32_t cur_stop_id = 0;
  bool first = true;
  lldb::SBEvent event;
  lldb::SBStream str_out;
  lldb::SBStream str_err;
  lldb::SBListener lis = debugger.GetListener();
  str_out.RedirectToFileHandle(stdout, false);
  str_err.RedirectToFileHandle(stderr, false);

  // Launch Process
  lldb::SBLaunchInfo li = target.GetLaunchInfo();
  lldb::SBError err;
  lldb::SBProcess proc = target.Launch(li, err);

  if (!proc.IsValid()) {
    str_out.Print(err.GetCString());
    return -1;
  }

  // Main loop
  // We run this loop untill the running process exits
  // The process can be killed and replaced by the user but the loop terminates
  // only when it exits
  while (true) {
    // Get current process
    // Required since the user can use commands such as 'r' durring debugging to
    // kill and start a new process
    proc = target.GetProcess();

    // Handle events
    while (lis.GetNextEvent(event)) {
      // This will print any process output
      debugger.HandleProcessEvent(proc, event, debugger.GetOutputFile(),
                                  debugger.GetErrorFile());
    }
    // Proc stopped
    if (proc.GetState() == lldb::eStateStopped) {

      // Get the stop id
      cur_stop_id = proc.GetStopID();

      // Check if this is a new stop
      // If the cur and last stop ids match then that means that the user
      // entered a command that did not restart the process we do not need to
      // write the source files again If the ids do not match that means that
      // means that this is a new stop and that we need to print the source code
      if ((cur_stop_id != last_proc_stop_id &&
           ((!Verbose && !debugger.GetAsync()) || debugger.GetAsync())) ||
          first) {
        last_proc_stop_id = cur_stop_id;
        first = false;

        str_out.Printf("Process %" PRIu64 " stopped\n", proc.GetProcessID());

        // My print source
        PrintSource(proc, str_out);
      }
      // Handle new command
      string buff;
      getline(cin, buff);

      lldb::SBCommandReturnObject result;
      debugger.GetCommandInterpreter().HandleCommand(buff.c_str(), result);
      if (Verbose) {
        str_out.Print(result.GetOutput());
        str_err.Print(result.GetError());
      }
      // If the user inputed the quit command a promt will appear asking if the
      // user wishes to kill the process
      // Did the user choose to kill the process ?
      if (result.GetStatus() == lldb::eReturnStatusQuit) {
        break;
      }

      // Check if we have quit with or without and exitcode
      if (debugger.GetCommandInterpreter().GetQuitStatus()) {
        break;
      }
    } else if (proc.GetState() == lldb::eStateExited) {
      // Exit status will be printed beforehand if verbose is on
      if (!Verbose || debugger.GetAsync()) {
        // If the process has exited print its status
        int exit_status = proc.GetExitStatus();
        const char *exit_desc = proc.GetExitDescription();
        printf("Process %" PRIu64 " exited with status = %i (0x%8.8x) %s\n",
               proc.GetProcessID(), exit_status, exit_status,
               exit_desc ? exit_desc : "");
      }
      break;
    }
  }
  // If the user has quit, kill the process
  if (proc.IsValid()) {
    proc.Kill();
  }
  return 0;
}
