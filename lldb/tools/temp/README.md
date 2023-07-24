# Basic Debug Tool
## Description:
Runs a basic wrapper for lldb. Sets the provided executable as the target, sets a breakpoint on the main function and starts debugging.
## Build:
  * cmake <other_build_options> -DLLVM_ENABLE_PROJECTS="clang;lldb" <other_build_options>
  * ninja lldb
  * If lldb is already built: ninja temp
    
  For more info about other build options read: https://llvm.org/docs/GettingStarted.html#checkout-llvm-from-git
## Run:
 $ temp <executable_path>

## Output:
   * Starts debugging the provided executable
   * Sets a breakpoint on the 'main' function
   * Prints any output that the executable prints
## Example
    $ temp ./basic.out
    INPUT: ./basic.out
    r
    Process 49371 launched: '<path>/test.out' (x86_64)
    Process 49371 running
    Process 49371 stopped
    thread #1: tid = 49371, 0x00005555555551df test.out`main at basic.cpp:11:7, name = 'test.out', stop reason = breakpoint 1.1
    frame #0: 0x00005555555551df test.out`main at basic.cpp:11:7
       8   	
       9   	int main(){
       10  	
    -> 11  	  int t  = 5;
       12  	  ding(&t);
       13  	
       14  	   std::cout<<"ding"<<std::endl;
    c
    Process 49371 resuming
    Process 49371 running
    ding
    Process 49371 exited with status = 0 (0x00000000) 


    

