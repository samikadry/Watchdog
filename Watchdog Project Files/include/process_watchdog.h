#ifndef __PROCESS_WATCHDOG_H__
#define __PROCESS_WATCHDOG_H__

/* Recieves program path and optional arguments. 
   Returns 0 for success, non-zero otherwise. 
   Place the watchdoog_run exec file in the same folder with your exec file. 
   Please don't use SIGUSR1 and SIGUSR2. */
int KeepMeAlive(int argc, char *const argv[]);

/* Returns 0 for success, non-zero otherwise. */
int DNR(void);

#endif /* __PROCESS_WATCHDOG_H__ */
