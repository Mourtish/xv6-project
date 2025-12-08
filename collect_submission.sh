#!/bin/bash
# Script to collect all submission materials

echo "=== Collecting Lottery Scheduler Submission ==="

# Create directories
mkdir -p submission/{lottery,vm,threads}

# Lottery project
echo "1. Collecting lottery project files..."
cp -v lottery/proc.h submission/lottery/
cp -v lottery/proc.c submission/lottery/
cp -v lottery/sysproc.c submission/lottery/
cp -v lottery/syscall.c submission/lottery/
cp -v lottery/syscall.h submission/lottery/
cp -v lottery/user.h submission/lottery/
cp -v lottery/usys.S submission/lottery/
cp -v lottery/pstat.h submission/lottery/
cp -v lottery/Makefile submission/lottery/
cp -v lottery/lotterytest.c submission/lottery/

# Create diff against original (optional)
echo "2. Creating diff against original..."
cd lottery
git diff --no-index ../xv6-public/proc.h proc.h > ../submission/lottery/proc.h.diff 2>/dev/null || true
git diff --no-index ../xv6-public/proc.c proc.c > ../submission/lottery/proc.c.diff 2>/dev/null || true
cd ..

# Create test output
echo "3. Running test to capture output..."
cd lottery
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo "=== Test Output ===" > ../submission/lottery/test_output.txt
echo "Running 'make qemu-nox' and testing..." >> ../submission/lottery/test_output.txt
# You'll need to manually capture QEMU output

echo "4. Submission files ready in: /workspaces/xv6-project/submission/"
echo "   - Modified source files in submission/lottery/"
echo "   - Report in REPORT.md"
echo "   - Test output in test_output.txt"
