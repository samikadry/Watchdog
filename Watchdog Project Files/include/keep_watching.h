#ifndef __KEEP_WATCHING_H__
#define __KEEP_WATCHING_H__

#include <sys/types.h> /* pid_t */
#include <stddef.h>    /* size_t */
#include <time.h>      /* time_t */

/* Sets signal handlers for SIGUSR1 and SIGUSR2, doesn't reset them :( */
/* Blocks till receiving SIGUSR2 */
/* Returns 0 for success, non-zero for fail */
int KeepWatching(char *const args[], pid_t pid_to_watch, size_t cycles_to_wait, 
                 size_t signals_interval);

#endif /* __KEEP_WATCHING_H__ */
