#ifndef _SYS_INFO_H
#define _SYS_INFO_H

#include "types.h" // for uint64

extern uint64 getFreemem(void); // Implement in kalloc.c
extern uint64 getNproc(void); // Implement in proc.c
extern uint64 getNopenFiles(void); // Implement in file.c
struct sysinfo
{
    uint64 freemem; // the number of bytes of free memory 
    uint64 nprocess;// the number of processes whose state is not UNUSED
    uint64 nopenfiles; // the number of opening files in whole system
};

#endif // _SYS_INFO_H