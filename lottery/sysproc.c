#include "pstat.h"
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"  


extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;


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

int
sys_settickets(void)
{
  int n;

  if(argint(0, &n) < 0)
    return -1;

  if(n < 1)  // Must have at least 1 ticket
    return -1;

  myproc()->tickets = n;
  return 0;
}

int
sys_getpinfo(void)
{
  struct pstat *ps;
  struct proc *p;
  int i;

  if(argptr(0, (void*)&ps, sizeof(*ps)) < 0)
    return -1;

  acquire(&ptable.lock);

  for(i = 0; i < NPROC; i++) {
    p = &ptable.proc[i];
    if(p->state != UNUSED) {
      ps->inuse[i] = 1;
      ps->tickets[i] = p->tickets;
      ps->pid[i] = p->pid;
      ps->ticks[i] = p->ticks;
    } else {
      ps->inuse[i] = 0;
      ps->tickets[i] = 0;
      ps->pid[i] = 0;
      ps->ticks[i] = 0;
    }
  }

  release(&ptable.lock);
  return 0;
}
