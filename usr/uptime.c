#include "types.h"
#include "stat.h"
#include "user.h"

int
main ()
{   
    int ticks = uptime();             // Get number of `ticks` from `uptime()` syscall
    printf(1, "%d ticks\n", ticks);   // Print `ticks` to stdout
    exit();
}