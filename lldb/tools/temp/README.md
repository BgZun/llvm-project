# Basic Debug Tool
## Description:
Runs the provided executable and prints the line number of the first line in the main method
## Build:
  * cmake <other_build_options> -DLLVM_ENABLE_PROJECTS="clang;lldb" <other_build_options>
  * ninja lldb
    
  For more info about other build options read: https://llvm.org/docs/GettingStarted.html#checkout-llvm-from-git
## Run:
 $ temp <executable_path>

## Output:
   * Prints the line number first line of main method (if it exists) when reached 
   * Prints any output that the executable prints
## Example
    $ temp ./basic.out
    INPUT: ./basic.out
    Process 64131 running
    Cur Line:11
    Process 64131 running
    
