#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* EXIT_FAILURE */
#include <unistd.h> /* getppid */

#include "keep_watching.h" /* KeepWatching, watchdog_args_ty */

int main(int argc, char *argv[])
{
    /* Check if the received arguments less than 2 */
    if (argc < 2)
    {
        /* Print an error message */
        fprintf(stderr, "USAGE: %s YOUR_EXECUTABLE [PARAMS]", argv[0]);

        /* Return with failure */
        return (EXIT_FAILURE);
    }

    /* Return with the status of KeepWatching function run */
    return (KeepWatching(argv + 1, getppid(), 5, 2));
}