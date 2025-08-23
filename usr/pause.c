#include "types.h"
#include "stat.h"
#include "user.h"

#define TICKS_PER_SECOND 10

int main(int argc, char **argv)
{
    if (argc != 2) {   // Check if second argument is present
        printf(2, "Pause duration not specified. (usage: pause <no-of-seconds>)\n");
        exit();
    }

    int pause_duration = atoi(argv[1]);

    // Convert `pause_duration` in seconds to `ticks`
    long ticks = TICKS_PER_SECOND * (long)pause_duration;
    
    // printf(1, "%d seconds\n", ticks);
    
    // Call sys_sleep for `ticks` ticks
    sleep((int)ticks);
    
    exit();
}