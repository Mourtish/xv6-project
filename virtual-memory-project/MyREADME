# COSC 361 Operating Systems - Project: xv6 Virtual Memory

## 1. Introduction
Implemented virtual memory protection features in xv6 operating system including:
- Null pointer dereference protection
- Page protection system calls (mprotect/munprotect)
- Page fault handling for illegal memory accesses

## 2. Files Modified

### 2.1 New Files Created
- `nulltest.c` - Tests null pointer dereference protection
- `nulltest2.c` - Comprehensive null pointer tests  
- `mprotect_test.c` - Tests mprotect/munprotect system calls
- `munprotect_test.c` - Tests munprotect functionality

### 2.2 Modified Files
1. **exec.c** (modified)
   - Changed `sz = 0;` to `sz = PGSIZE;` (line 34)
   - Makes first page (0x0-0xFFF) invalid/unmapped
   - Programs load starting at 0x1000 instead of 0x0

2. **vm.c** (modified)
   - `copyuvm()`: Changed `panic("copyuvm: page not present")` to `continue;`
   - Allows fork() to skip non-present pages (like page 0)
   - `walkpgdir()`: Removed `static` keyword to make it accessible from sysproc.c

3. **defs.h** (modified)
   - Added typedefs: `typedef uint pte_t;` and `typedef uint pde_t;`
   - Added declaration: `pte_t* walkpgdir(pde_t*, const void*, int);`

4. **sysproc.c** (modified)
   - Added `change_protection()` helper function
   - Added `sys_mprotect()`: Makes pages read-only
   - Added `sys_munprotect()`: Restores write permissions

5. **syscall.h** (modified)
   - Added system call numbers: `SYS_mprotect` (22) and `SYS_munprotect` (23)

6. **syscall.c** (modified)
   - Added extern declarations for new system calls
   - Added entries to system calls table

7. **user.h** (modified)
   - Added function prototypes: 
     ```c
     int mprotect(void *addr, int len);
     int munprotect(void *addr, int len);
     ```

8. **usys.S** (modified)
   - Added system call stubs for mprotect and munprotect

9. **Makefile** (modified)
   - Added test programs to UPROGS: `_nulltest`, `_nulltest2`, `_mprotect_test`, `_munprotect_test`

## 3. Implementation Details

### 3.1 Null Pointer Protection
- First page (0x0-0xFFF) marked as not present
- Programs load at 0x1000 instead of 0x0
- Accessing null pointer causes **page fault (trap 14)**
- Kernel kills process on page fault (automatic in xv6)

### 3.2 Page Protection System Calls
- `mprotect(void *addr, int len)`: Makes `len` pages starting at `addr` read-only
- `munprotect(void *addr, int len)`: Restores write permissions
- Validates: page alignment, valid address range, positive length
- Flushes TLB after changing protections using `lcr3(V2P(pgdir))`

### 3.3 Page Fault Handling
- xv6 automatically kills process on page fault
- Write to protected page → trap 14 (page fault) → process killed
- Null pointer access → trap 14 → process killed

### 3.4 Protection Inheritance
- `copyuvm()` copies page table entries with all flags (including PTE_W)
- Child processes inherit parent's page protections automatically

## 4. System Calls Added

### 4.1 `int mprotect(void *addr, int len)`
- Makes `len` pages starting at `addr` read-only
- Returns 0 on success, -1 on error (bad alignment, invalid address, len ≤ 0)
- Removes PTE_W (write) bit from page table entries

### 4.2 `int munprotect(void *addr, int len)`
- Restores write permission to `len` pages starting at `addr`
- Returns 0 on success, -1 on error
- Adds PTE_W bit to page table entries

## 5. Testing

### 5.1 Null Pointer Test (`nulltest2.c`)
```console
$ nulltest2
Testing different null pointer accesses...
Test 1: Reading from null pointer...
pid 3 nulltest2: trap 14 err 4 on cpu 0 eip 0x102b addr 0x0--kill proc
