#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->tgid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void)
{
  void(*fcn)(void*, void *);
  int arg1, arg2, stack;

  if(argptr(0, (void*) &fcn, sizeof(void*)) < 0)
    return -1;
  
  if((argptr(1, (void*) &arg1, sizeof(void*)) < 0) || (argptr(2, (void*) &arg2, sizeof(void*)) < 0) || (argptr(3, (void*) &stack, sizeof(void*)) < 0))
    return -1;

  return clone((void*) fcn, (void*) arg1, (void*) arg2, (void*) stack);
}

int
sys_join(void)
{
  void **stack;
  int stackArg;

  stackArg = argint(0, &stackArg);
  stack = (void **)stackArg;

  return join(stack);
}

int
sys_gettid(void)
{
  return myproc()->pid;
}