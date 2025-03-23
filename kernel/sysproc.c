#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

#include "sysinfo.h" // for sysinfo

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// our lab

uint64
sys_trace(void)
{
  int mask;

  // mask from int main in trace.c will store the argument[1] from user space (for example is 32) into register a0 base on convention, 
  // so use argint(0, &mask) that load the value of a0 to mask
  argint(0, &mask);

  // myproc() return current process, so assigment
  myproc()->trace_mask = mask;

  // okay, from now, if any system calls got this mask (which reference from #define SYS_) 
  // will be traced by be printed information when syscall() function be called.
  return 0;
}

uint64
sys_sysinfo(void)
{
  struct sysinfo in4;

  in4.freemem = getFreemem();
  in4.nopenfiles = getNopenFiles();
  in4.nprocess = getNproc();

  uint64 addr;
  argaddr(0, &addr); // extract the address of in4 from user space

  if (copyout(myproc()->pagetable, addr, (char *)&in4, sizeof(in4)) < 0) // copy back information for user space
  {
    return -1; // if < 0 return failed (-1)
  }

  return 0;
}

int sys_pgaccess(void) {
  uint64 start;
  int npages;
  uint64 abitsaddr;
  uint64 va;
  uint64 mask;
  uint32 abits;  // Changed to uint32 to match user-space
  int page_num;

  struct proc *p = myproc();
  argaddr(0, &start);
  argint(1, &npages);
  argaddr(2, &abitsaddr);

  // printf("sys_pgaccess: start=%p, npages=%d, abitsaddr=%p\n", start, npages, abitsaddr);

  if (npages > 64) {
    // printf("npages too large: %d\n", npages);
    return -1;
  }

  mask = 1;
  abits = 0;
  page_num = 0;
  printf("sys_pgaccess: checking pages for start=%p, npages=%d\n", (void*)start, npages);
  
  for (va = start; va < start + PGSIZE * npages; va += PGSIZE) {
    pte_t *pte = walk(p->pagetable, va, 0);
    if (pte == 0) {
      // printf("walk failed for va=%p\n", va);
      printf("sys_pgaccess: walk failed for va=%p (page %d)\n", (void*)va, page_num);
      return -1;
    }
    // printf("va=%p, pte=%p, pte_val=%p\n", (void*)va, (void*)pte, *pte);
    if (*pte & PTE_A) {
      // printf("PTE_A set for va=%p\n", va);
      printf("sys_pgaccess: page %d accessed (va=%p)\n", page_num, (void*)va);
      abits |= mask;
      *pte = *pte & (~PTE_A);
    }
    mask <<= 1;
    page_num++;
  }

  // printf("abits=%x\n", abits);
  if (copyout(p->pagetable, abitsaddr, (char *)&abits, sizeof(uint32)) < 0) {
    printf("copyout failed\n");
    return -1;
  }
  return 0;
}

