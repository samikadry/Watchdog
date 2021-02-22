#define _POSIX_C_SOURCE  200112L
#include <assert.h>  /* assert */
#include <stdio.h>   /* printf */
#include <stdlib.h>  /* EXIT_FAILURE, calloc, free, getenv, setenv */
#include <pthread.h> /* pthread_create, pthread_t */
#include <errno.h>   /* errno */
#include <unistd.h>  /* getpid */
#include <signal.h>  /* kill, SIGUSR2 */
#include <errno.h>   /* errno */

#include "valid_utils.h"   /* RETURN_IF_BAD, ExitIfBad */
#include "keep_watching.h" /* watchdog_args_ty, KeepWatching */

#include "process_watchdog.h" /* The implemented API */

/* Using at validation that only kepping alive one process */
#ifndef NDEBUG

static int is_keeping_process = 0;

#endif

static pthread_t thread_id = 0;
static char **g_args = NULL;

/* Wrapping functio of KeepWatching function */
static void *KeepWatchRunner(void *args);

/* Creates an arguments array like argv that received from user program, but
   adding a one place for our executable file path. Return the new args */
static int ArgsCreate(int argc, char *const argv[]);


int KeepMeAlive(int argc, char *const argv[])
{
	int err = 0;

    /* Arguments validation */
    assert(1 <= argc);
	assert(argv);

	/* Validate that only kepping alive one process */
	assert(1 == ++is_keeping_process);
	
	/* Create a new args with argv + 1 size, and put the exec file path in it */
	if (NULL == getenv("first_exec"))
	{
		RETURN_IF_BAD(0 == ArgsCreate(argc, (char **)argv), 
					  "\nArgsCreate() is failed", EXIT_FAILURE);

		RETURN_IF_BAD(0 == setenv("first_exec", "0", 1),"\nsetenv() is failed",
		              errno);
	}

	do /* Try to create a thread of watchdog */
	{
        err = pthread_create(&thread_id, NULL, KeepWatchRunner, g_args);
		
        /* Validate the thread creation */
		RETURN_IF_BAD(0 == err || EAGAIN == err, "\npthread_create() is failed",
                      err); 

	}while (err); /* While EAGAIN */

    /* Return SUCCESS status */
	return (err);
}

int DNR(void)
{
	/* Send SIGUSR2 to current process and validate it */
	RETURN_IF_BAD(0 == kill(getpid(), SIGUSR2), "\nkill() is failed",
                  EXIT_FAILURE);

	/* Validate the thread termination */
	RETURN_IF_BAD(0 == pthread_join(thread_id, NULL), "\npthread_join() is "
	              "failed", EXIT_FAILURE);

	/* Free the args allocated memory */
	free(g_args);
	
    /* Return SUCCESS status */
	return (0);
}

static void *KeepWatchRunner(void *args)
{
	/* Run the KeepWatching function with appropriate arguments */
	KeepWatching((char **)args, getppid(), 5, 2);

	return (NULL);
}

static int ArgsCreate(int argc, char *const argv[])
{
	int i = 0;
	
	/* Allocate memory for "g_args" */
	g_args = (char **)calloc(argc + 2, sizeof(char *));

	/* Validate memory allocation */
	RETURN_IF_BAD(NULL != g_args, "\ncalloc() is failed", EXIT_FAILURE);

	/* Initialize the "g_args" */
	g_args[0] = "./run_watchdog";
	for (i = 1; i <= argc; ++i)
    {
        g_args[i] = argv[i - 1];    
    }

	g_args[i] = NULL;

	/* Return SUCCESS status */
	return (0);
}
