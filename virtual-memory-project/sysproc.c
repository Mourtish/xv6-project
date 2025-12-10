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
// Helper function to change protection for a range of pages
static int
change_protection(pde_t *pgdir, uint addr, int len, int protect)
{
  pte_t *pte;
  
  // Check alignment
  if(addr % PGSIZE != 0)
    return -1;
    
  // Check length
  if(len <= 0)
    return -1;
    
  for(int i = 0; i < len; i++, addr += PGSIZE) {
    // Get page table entry
    pte = walkpgdir(pgdir, (void*)addr, 0);
    if(!pte || !(*pte & PTE_P) || !(*pte & PTE_U)) {
      // Page not present or not user-accessible
      return -1;
    }
    
    // Modify protection
    if(protect) {
      *pte &= ~PTE_W;  // Remove write permission (make read-only)
    } else {
      *pte |= PTE_W;   // Add write permission
    }
  }
  
  // Flush TLB
  lcr3(V2P(pgdir));
  
  return 0;
}
int
sys_mprotect(void)
{
  uint addr;
  int len;
  
  if(argint(0, (int*)&addr) < 0 || argint(1, &len) < 0)
    return -1;
    
  return change_protection(myproc()->pgdir, addr, len, 1);
}

int
sys_munprotect(void)
{
  uint addr;
  int len;
  
  if(argint(0, (int*)&addr) < 0 || argint(1, &len) < 0)
    return -1;
    
  return change_protection(myproc()->pgdir, addr, len, 0);
}
