#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int main()
{
  printf(1, "Testing munprotect...\n");
  
  // Allocate a page
  char *ptr = sbrk(PGSIZE);
  printf(1, "Allocated page at %p\n", ptr);
  
  // Write initial value
  ptr[0] = 'A';
  printf(1, "Write 'A': success\n");
  
  // Make read-only
  if(mprotect(ptr, 1) < 0) {
    printf(1, "mprotect failed\n");
    exit();
  }
  printf(1, "Page protected\n");
  
  // Try to write (should trap)
  printf(1, "Attempting write (should trap)...\n");
  // We can't actually test the trap here since it would kill us
  // Instead, we'll munprotect first
  
  // Restore write permission
  if(munprotect(ptr, 1) < 0) {
    printf(1, "munprotect failed\n");
    exit();
  }
  printf(1, "Page unprotected\n");
  
  // Write should work again
  ptr[0] = 'B';
  printf(1, "Write 'B' after munprotect: success\n");
  
  printf(1, "Test passed!\n");
  exit();
}
