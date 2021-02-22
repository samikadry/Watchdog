# Watchdog Project

## How to run ?
### open the terminal from the "Watchdog Project Files" file and run the next command : 
main_program.out ./exec/run_watchdog.out ./lib/*


## Program description :
### this program's interface includes two functions: KeepMeAlive() and DNR(). 

### KeepMeAlive():
the goal of this function is to guarantee executing of whole the code that exist after it. 

### DNR():
this function is stopping the action of KeepMeAlive() function.


## Usage Example:

int main()
{
    
    // user code

    KeepMeAlive();
    
    // user important code that must executes without any interrupts
    
    DNR();
    
    // user code that didn't so important to be executed

    return 0;
}
