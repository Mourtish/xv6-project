#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int main()
{
  printf(1, "Testing mprotect/munprotect...\n");
  
  // Allocate 2 pages
  char *ptr = sbrk(PGSIZE * 2);
  printf(1, "Allocated 2 pages at address %p\n", ptr);
  
  // Write to page (should work)
  ptr[0] = 'A';
  printf(1, "Write 'A' to page: success\n");
  
  // Make page read-only
  printf(1, "Calling mprotect...\n");
  if(mprotect(ptr, 1) == 0) {
    printf(1, "mprotect succeeded\n");
  } else {
    printf(1, "mprotect failed\n");
    exit();
  }
  
  // Try to write to protected page (should cause page fault)
  printf(1, "Attempting to write to protected page...\n");
  ptr[0] = 'B';  // Should trap here!
  printf(1, "Write 'B': succeeded (SHOULD NOT SEE THIS!)\n");
  
  // Restore write permission
  printf(1, "Calling munprotect...\n");
  if(munprotect(ptr, 1) == 0) {
    printf(1, "munprotect succeeded\n");
  } else {
    printf(1, "munprotect failed\n");
  }
  
  // Write should work again
  ptr[0] = 'C';
  printf(1, "Write 'C' after munprotect: success\n");
  
  printf(1, "Test completed\n");
  exit();
}
