#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

void
run_child(int tickets, char name)
{
    settickets(tickets);
    printf(1, "Process %c started: PID=%d, tickets=%d\n", name, getpid(), tickets);
    
    // Run for a long time
    int counter = 0;
    while(1) {
        counter++;
        if(counter % 1000000 == 0) {
            // Just to show it's alive
        }
    }
    
    exit();
}

int
main(int argc, char *argv[])
{
    struct pstat st;
    int pidA, pidB, pidC;
    int i;
    
    printf(1, "\n=== Lottery Scheduler Test ===\n\n");
    printf(1, "Creating 3 processes with 30:20:10 tickets ratio\n\n");
    
    // Create three processes
    pidA = fork();
    if(pidA == 0) {
        run_child(30, 'A');
    }
    
    pidB = fork();
    if(pidB == 0) {
        run_child(20, 'B');
    }
    
    pidC = fork();
    if(pidC == 0) {
        run_child(10, 'C');
    }
    
    printf(1, "Process A: PID %d (30 tickets)\n", pidA);
    printf(1, "Process B: PID %d (20 tickets)\n", pidB);
    printf(1, "Process C: PID %d (10 tickets)\n", pidC);
    
    printf(1, "\nLetting processes run for 3 seconds...\n");
    sleep(300);  // 3 seconds
    
    // Get statistics
    if(getpinfo(&st) < 0) {
        printf(1, "ERROR: getpinfo failed\n");
        exit();
    }
    
    // Find our processes
    int ticksA = 0, ticksB = 0, ticksC = 0;
    
    for(i = 0; i < NPROC; i++) {
        if(st.inuse[i]) {
            if(st.pid[i] == pidA) {
                ticksA = st.ticks[i];
            } else if(st.pid[i] == pidB) {
                ticksB = st.ticks[i];
            } else if(st.pid[i] == pidC) {
                ticksC = st.ticks[i];
            }
        }
    }
    
    printf(1, "\nResults after 3 seconds:\n");
    printf(1, "PID\tTickets\tTicks\t%% CPU\n");
    printf(1, "---\t-------\t-----\t------\n");
    
    int total = ticksA + ticksB + ticksC;
    
    if(total > 0) {
        // Fixed printf statements - no % in format string
        printf(1, "%d\t%d\t%d\t%.1f\n", pidA, 30, ticksA, (float)ticksA*100/total);
        printf(1, "%d\t%d\t%d\t%.1f\n", pidB, 20, ticksB, (float)ticksB*100/total);
        printf(1, "%d\t%d\t%d\t%.1f\n", pidC, 10, ticksC, (float)ticksC*100/total);
        
        printf(1, "\nExpected: 50%% : 33%% : 17%% (3:2:1 ratio)\n");
        printf(1, "Actual: %.1f%% : %.1f%% : %.1f%%\n",
               (float)ticksA*100/total,
               (float)ticksB*100/total, 
               (float)ticksC*100/total);
        
        // Check ratios
        float perTicketA = (float)ticksA / 30;
        float perTicketB = (float)ticksB / 20;
        float perTicketC = (float)ticksC / 10;
        
        printf(1, "\nTicks per ticket:\n");
        printf(1, "A: %.2f, B: %.2f, C: %.2f\n", perTicketA, perTicketB, perTicketC);
        
        // All should be roughly equal
        float avg = (perTicketA + perTicketB + perTicketC) / 3;
        float tolerance = 0.5; // 50% tolerance
        
        if(perTicketA > avg * (1 - tolerance) && perTicketA < avg * (1 + tolerance) &&
           perTicketB > avg * (1 - tolerance) && perTicketB < avg * (1 + tolerance) &&
           perTicketC > avg * (1 - tolerance) && perTicketC < avg * (1 + tolerance)) {
            printf(1, "\n✓ PASS: Lottery scheduler working correctly!\n");
        } else {
            printf(1, "\n✗ FAIL: Ratios not proportional to tickets\n");
        }
    }
    
    printf(1, "\nKilling test processes...\n");
    kill(pidA);
    kill(pidB);
    kill(pidC);
    wait();
    wait();
    wait();
    
    printf(1, "\n=== Test complete ===\n");
    exit();
}
