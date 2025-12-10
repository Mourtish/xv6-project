#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
  printf(1, "Testing different null pointer accesses...\n");
  
  // Test 1: Read from null
  printf(1, "Test 1: Reading from null pointer...\n");
  int *p1 = 0;
  int value = *p1;
  printf(1, "Read value: %d\n", value);
  
  // Test 2: Write to null  
  printf(1, "Test 2: Writing to null pointer...\n");
  *p1 = 42;
  printf(1, "Write successful\n");
  
  // Test 3: Execute at null
  printf(1, "Test 3: Calling function at null...\n");
  void (*func)() = 0;
  func();
  
  printf(1, "All tests passed (shouldn't happen)\n");
  exit();
}
