#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



// the commandline will be
// trace (mask number) (user function)

// if you do const char*argv[], you will get warning when make qemu because exec() function expected char** not const char**
int main(int agrc, char *argv[])
{
    if(agrc < 3)
    {
        printf("Wrong arguments input \n The commandline should be: trace (mask number) (user function)!\n");
        exit(1); // exit the current process, does not return;
    }
    
    int mask = atoi(argv[1]); // got mask
    trace(mask); // trace(mask) will call systemcall thanks to entry("trace") in usys.pl
    exec(argv[2], &argv[2]);
    return 0;
}