#include <stdio.h>  /* printf */
#include <unistd.h> /* getpid */

#include "process_watchdog.h"

int main(int argc, char *argv[])
{
    int watchdog_res = 0, i = 0;

    watchdog_res = KeepMeAlive(argc, argv);

    for (i = 0; i < 100; ++i)
    {
        printf("User process id: %ld", getpid());

        sleep(1);
    } 

    DNR();

    printf("\nUser process terminating ...\n");

    return (0);
}