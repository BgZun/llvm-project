Basic Debug Tool
Description:
  Prints the line number of the first line in the main method
Build:
  Run  :  cmake <other build options> -DLLVM_ENABLE_PROJECTS="clang;lldb" <other_build_options>
          ninja lldb
  For more info about other build options read: https://llvm.org/docs/GettingStarted.html#checkout-llvm-from-git
To Run:
 $ <bulld_dir/bin>/temp <executable with debug info>

Output:
  Prints the line number first line of main method (if it exists) when reached 
  Prints any output that the executable prints
