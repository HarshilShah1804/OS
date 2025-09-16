// Add necessary header files here
#include "types.h"
#include "user.h"
#include "stat.h"

#define PGSIZE 4096
#define MAXVA (1ULL << (9 + 9 + 9 + 12 - 1))
#define PTE2PA(pte)     (((int)(pte)) >> 10 << 12)
#define PTE_FLAGS(pte)  (((int)(pte)) & 0x3FF)
#define PTE_V (1ULL << 0)
#define PTE_R (1ULL << 1)
#define PTE_W (1ULL << 2)
#define PTE_X (1ULL << 3)
#define PTE_U (1ULL << 4)

#define SUPERPGROUNDUP(a) (((a)+512*PGSIZE-1) & ~(512*PGSIZE-1))

#define N (8 * (1 << 20))

typedef uint32 pte_t;
extern uint32 pgpte(void *va);


void print_pt();
void print_kpt();
void ugetpid_test();
void superpg_test();

int main(int argc, char *argv[])
{
  // print_pt();
  ugetpid_test();
  print_kpt();
  // superpg_test();
  printf(1, "pttest: all tests succeeded\n");
  exit();
}

char *testname = "";

void
err(char *why)
{
  printf(2, "pttest: %s failed: %s, pid=%d\n", testname, why, getpid());
  exit();
}

void
print_pte(int va)
{
    uint32 pte = (uint32) pgpte((void *) va);
    printf(1,"va 0x%lx pte 0x%lx pa 0x%lx perm 0x%lx\n", va, pte, PTE2PA(pte), PTE_FLAGS(pte));
}

void
print_pt()
{
  printf(1, "print_pt starting\n");
  for (int i = 0; i < 10; i++) {
    print_pte(i * PGSIZE);
  }
  int top = MAXVA/PGSIZE;
  // printf(1, "%d", top);
  for (int i = top-10; i < top; i++) {
    print_pte(i * PGSIZE);
  }
  printf(1, "print_pt: OK\n");
}

void
ugetpid_test()
{
  int i;

  printf(1, "ugetpid_test starting\n");
  testname = "ugetpid_test";

  for (i = 0; i < 64; i++) {
    int ret = fork();
    wait();
    if (ret != 0) {
      wait();
      continue;
    }
    if (getpid() != ugetpid())
      err("missmatched PID");
    exit();
  }
  printf(1, "ugetpid_test: OK\n");
}

void
print_kpt()
{
  printf(1, "print_kpt starting\n");
  kpt(); // Implement in vm.c to access kernel pagetable
  printf(1, "print_kpt: OK\n");
}


void
supercheck(int s)
{
  pte_t last_pte = 0;

  for (int p = s;  p < s + 512 * PGSIZE; p += PGSIZE) {
    uint32 pte = pgpte((void *) p);
    if(pte == 0)
      err("no pte");
    if ((int) last_pte != 0 && pte != last_pte) {
        err("pte different");
    }
    if((pte & PTE_V) == 0 || (pte & PTE_R) == 0 || (pte & PTE_W) == 0){
      err("pte wrong");
    }
    last_pte = pte;
  }

  for(int i = 0; i < 512; i += PGSIZE){
    *(int*)(s+i) = i;
  }

  for(int i = 0; i < 512; i += PGSIZE){
    if(*(int*)(s+i) != i)
      err("wrong value");
  }
}

void
superpg_test()
{
  int pid;

  printf(1, "superpg_test starting\n");
  testname = "superpg_test";
  
  char *end = sbrk(N);
  if (end == 0 || end == (char*)0xffffffffffffffff)
    err("sbrk failed");
  
  int s = SUPERPGROUNDUP((int) end);
  supercheck(s);
  if((pid = fork()) < 0) {
    err("fork");
  } else if(pid == 0) {
    supercheck(s);
    exit();
  } else {
    wait();
  }
  printf(1, "superpg_test: OK\n");  
}
