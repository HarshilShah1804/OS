#include "types.h"
#include "stat.h"
#include "user.h"

// The pstat struct definition now comes from stat.h,
// which is included by user.h
#define NPROC 64 // Max number of processes

int main(int argc, char *argv[])
{
    struct pstat ptable[NPROC];
    int num_procs, i;

    num_procs = getprocs(ptable);

    if (num_procs < 0) {
        printf(2, "ps: error getting process table\n");
        exit();
    }

    // Adjusted header with an extra tab for the 'State' column
    printf(1, "PID\tPPID\tState\t\tName\tSyscalls\n");
    printf(1, "---\t----\t-----\t\t----\t--------\n");

    for (i = 0; i < num_procs; i++) {
        if (ptable[i].inuse) {
            // Check the length of the state string to decide on the number of tabs
            if (strlen(ptable[i].state) < 8) {
                printf(1, "%d\t%d\t%s\t\t%s\t%d\n",
                       ptable[i].pid,
                       ptable[i].ppid,
                       ptable[i].state,
                       ptable[i].name,
                       ptable[i].syscalls);
            } else {
                printf(1, "%d\t%d\t%s\t%s\t%d\n",
                       ptable[i].pid,
                       ptable[i].ppid,
                       ptable[i].state,
                       ptable[i].name,
                       ptable[i].syscalls);
            }
        }
    }

    exit();
}
