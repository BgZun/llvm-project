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
   * No verbose option
     ```
     $ temp ./test.out
     INPUT: ./test.out
     Process 9673 stopped
     thread #1: tid = 9673, 0x00005555555551df test.out`main at basic.cpp:11:7, name = 'test.out', stop reason = breakpoint 1.1
     frame #0: 0x00005555555551df test.out`main at basic.cpp:11:7
        8   	
        9   	int main(){
        10  	
     -> 11  	  int t  = 5;
        12  	  ding(&t);
        13  	
        14  	   std::cout<<"ding"<<std::endl;
     b ding
     c
     Process 9673 stopped
     thread #1: tid = 9673, 0x00005555555551b8 test.out`ding(t=0x00007fffffffde38) at basic.cpp:6:12, name = 'test.out', stop reason = breakpoint 2.1
     frame #0: 0x00005555555551b8 test.out`ding(t=0x00007fffffffde38) at basic.cpp:6:12
        3   	using namespace std;
        4   	
        5   	int ding(int* t){
     -> 6   	  return (*t)++;
        7   	}
        8   	
        9   	int main(){
        c
        Process 9673 exited with status = 0 (0x00000000)
     ```
   * With verbose option
     ```
     $ temp ./test.out -verbose
     INPUT: ./test.out
     Process 10863 stopped
     thread #1: tid = 10863, 0x00005555555551df test.out`main at basic.cpp:11:7, name = 'test.out', stop reason = breakpoint 1.1
     frame #0: 0x00005555555551df test.out`main at basic.cpp:11:7
        8   	
        9   	int main(){
        10  	
     -> 11  	  int t  = 5;
        12  	  ding(&t);
        13  	
        14  	   std::cout<<"ding"<<std::endl;
     b ding
     Breakpoint 2: where = test.out`ding(int*) + 8 at basic.cpp:6:12, address = 0x00005555555551b8
     c
     Process 10863 resuming
     Process 10863 stopped
     * thread #1, name = 'test.out', stop reason = breakpoint 2.1
         frame #0: 0x00005555555551b8 test.out`ding(t=0x00007fffffffde38) at basic.cpp:6:12
        3   	using namespace std;
        4   	
        5   	int ding(int* t){
     -> 6   	  return (*t)++;
        7   	}
        8   	
        9   	int main(){
     Process 10863 stopped
     thread #1: tid = 10863, 0x00005555555551b8 test.out`ding(t=0x00007fffffffde38) at basic.cpp:6:12, name = 'test.out', stop reason = breakpoint 2.1
     frame #0: 0x00005555555551b8 test.out`ding(t=0x00007fffffffde38) at basic.cpp:6:12
        3   	using namespace std;
        4   	
        5   	int ding(int* t){
     -> 6   	  return (*t)++;
        7   	}
        8   	
        9   	int main(){
     c
     Process 10863 resuming
     Process 10863 exited with status = 0 (0x00000000)

     ```

    



    

