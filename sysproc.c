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
  return myproc()->pid;
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

// for pa3
int 
sys_mmap(void)
{
  //arguments
  int arg[6];
  for (int i = 0; i < sizeof(arg)/sizeof(arg[0]); i++)
    if (argint(i, &arg[i]) < 0)
      goto arg_error;
  return mmap((uint)arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);
arg_error:
  return -1;
}

int 
sys_munmap(void)
{
  int arg;
  if (argint(0, &arg) < 0)
    goto arg_error; 
  return munmap(arg);
arg_error:
    return -1;
}

int 
sys_freemem(void)
{
  return freemem();
}

// pagefault handler called from trap
int 
sys_pgflt(void)
{
  uint err;
  if (argint(0, (int*)&err) < 0)
    goto arg_error; 
  return pgflt(err);
arg_error:
    return -1;
}

// get information about protection 
// on specific memory address
// if address dne at mmap_area, return -1
int 
sys_getprot(void)
{ 
  uint arg;
  if (argint(0, (int*)&arg) < 0)
    goto arg_error; 
  return getprot(arg);
arg_error:
    return -1;
}

// print mtable
int 
sys_pm(void)
{
	return pm();
}

// Ctrl+C
int
sys_fgproc(void)
{
	return fgproc();
}

int 
sys_procdump(void)
{
	procdump();
	return 1;
}
