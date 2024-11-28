#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va;
  uint64 mask_addr;
  int len;

  argaddr(0, &va);
  argint(1, &len);
  argaddr(2, &mask_addr);

  int mask = 0;
  
  if(va >= MAXVA)
    return 0;

  if (len > 64) {
    len = 64;
  }
  
  pte_t *pte;  
  for (int i = 0; i < len; i++) {
    pte = walk(myproc()->pagetable, va + i * PGSIZE, 0);
    if (pte == 0)
      return -1;

    // 检查访问位 PTE_A 是否被设置
    if (*pte & PTE_A) {
      mask |= (1 << i);
    }

    // 手动清除 PTE_A 标志，硬件不会自动清除
    *pte &= ~PTE_A;
  }
  if (copyout(myproc()->pagetable, mask_addr, (char *)&mask, sizeof(mask)) < 0)
    return -1;

  return 0;
}
#endif

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
