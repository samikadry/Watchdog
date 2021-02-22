#define _POSIX_C_SOURCE 199309L
#include <assert.h> /* assert */
#include <unistd.h> /* pid_t, getppid, fork, execv */
#include <stdlib.h> /* EXIT_FAILURE, atexit, atoi */
#include <stdio.h>  /* time */
#include <signal.h> /*sigaction, kill, sig_atomic_t, SIGUSR1, SIGUSR2, SIGKILL*/
#include <errno.h>  /* errno */
 
#include "valid_utils.h"   /* RETURN_IF_BAD, ExitIfBadPerror */
#include "keep_watching.h" /* The implemented API */
#include "scheduler.h"     /* SchedulerCreate, SchedulerAdd, SchedulerClear,
                              SchedulerRun, SchedulerDestroy, u_id_t */

typedef struct 
{
    char **args;           /* The argv (received arguments array) */
    size_t cycles_to_wait; /* Number of maximum allowed cycles without 
                              receiving a signal from the other process */
}task_args_ty;

static volatile sig_atomic_t g_time_without_sigs = 0;
static volatile sig_atomic_t g_dnr_signal = 0;
static volatile pid_t  g_pid_to_watch = 0;
static scheduler_t *g_scheduler = NULL;

static void Sigusr1Handler(int sig, siginfo_t *info, void *ucontext);
static void Sigusr2Handler(int sig, siginfo_t *info, void *ucontext);
static op_f_status ValidateAndReviveTask(void *args);
static op_f_status SendSignalTask(void *args);
static void CleanupResources(void);

int KeepWatching(char *const args[], pid_t pid_to_watch, size_t cycles_to_wait, 
                 size_t signals_interval)
{
    struct sigaction sigusr1_action, sigusr2_action;
    task_args_ty task_args;
    u_id_t curr_uid;

    /* Initialize the task function arguments */
    task_args.args = (char **)args;
    task_args.cycles_to_wait = cycles_to_wait;
    g_pid_to_watch = pid_to_watch;

	/* Set SA_SIGINFO in sa_flags to use sa_sigaction instead of sa_handler */
	sigusr1_action.sa_flags = SA_SIGINFO;
    sigusr2_action.sa_flags = SA_SIGINFO;

	/* Update the signal handler functions */
	sigusr1_action.sa_sigaction = Sigusr1Handler;
    sigusr2_action.sa_sigaction = Sigusr2Handler;

    /* Update SIGUSR1 and SIGUSR2 sigaction, validate it */
    RETURN_IF_BAD(0 == sigaction(SIGUSR1, &sigusr1_action, 0),
                  "\nsigaction() is failed", EXIT_FAILURE);

    RETURN_IF_BAD(0 == sigaction(SIGUSR2, &sigusr2_action, 0), 
                  "\nsigaction() is failed", EXIT_FAILURE);

    /* Create a scheduler, and validate it */
    RETURN_IF_BAD(NULL != (g_scheduler = SchedulerCreate()), 
                  "\nSchedulerCreate() is failed", EXIT_FAILURE);
	
    /* Add tasks to scheduler, in error case cleanup resources and return */
    /* Create task 1 that Sampling every 2 seconds if received a signal */
    curr_uid = SchedulerAdd(g_scheduler, ValidateAndReviveTask, &task_args, 
                            time(NULL), signals_interval);

    CLEAN_AND_RETURN_IF_BAD(!UidsIsEquals(curr_uid, ErrUidCreate()), 
                  "\nSchedulerAdd() is failed", EXIT_FAILURE, CleanupResources);

    /* Create task 2 that sending a signal every 2 second */
    curr_uid = SchedulerAdd(g_scheduler, SendSignalTask, &task_args, time(NULL),
                            signals_interval);

    CLEAN_AND_RETURN_IF_BAD(!UidsIsEquals(curr_uid, ErrUidCreate()),
                  "\nSchedulerAdd() is failed", EXIT_FAILURE, CleanupResources);

    /* Setup atexit function to cleanup the scheduler and tasks resources */
    RETURN_IF_BAD(0 == atexit(CleanupResources), "\natexit() is failed",
                  EXIT_FAILURE);

    /* Run the scheduler */
    SchedulerRun(g_scheduler);

    /* Validate receiving SIGUSR2 signal (DNR function is called) */
    assert(0 != g_dnr_signal);

    /* Return SUCCESS status */
    return (0);
}

static op_f_status ValidateAndReviveTask(void *args)
{
    task_args_ty *task_args = (task_args_ty *)args;

    /* Check if the time without receiving signals less than the max allowed */
    if ((sig_atomic_t)task_args->cycles_to_wait >= g_time_without_sigs)
    {
        /* Increment the time without receiving signals */
        __sync_add_and_fetch(&g_time_without_sigs, 1);
    }
    else /* Otherwise */
    {
        /* If there is another process to contact with it */
        if (1 < g_pid_to_watch)
        {
            /* Kill the other process by sending SIGKILL signal to it */
            ExitIfBadPerror((0 == kill(g_pid_to_watch, SIGKILL)), 
                            "\nkill() is failed", errno);
        }

        /* Create a watchdog/user process */
        g_pid_to_watch = fork();

        /* Validate the process creation */
        ExitIfBadPerror(-1 != g_pid_to_watch, "\nfork() is failed", errno);

        if (0 == g_pid_to_watch) /* If child */
        {
            /* Execute the watchdog/user process with appropriate program */
            ExitIfBadPerror(-1 != execv(task_args->args[0], task_args->args),
                            "\nexecv() is failed", errno);
        }
        else /* If parent */
        {
            /* Reset the time without receiving signals */
            g_time_without_sigs = 0;
        }
    }

    /* If DNR called by user */
    if (1 == g_dnr_signal)
    {
        /* Stop the scheduler running */
        SchedulerStop(g_scheduler);

        /* Return SUCCESS status */
        return (SUCCESSED_END);
    }

    /* return REPEAT status */
    return (REPEAT);
}

static op_f_status SendSignalTask(void *args)
{
    /* If there is another process to contact with it */
    if (1 < g_pid_to_watch)
    {
        /* send a SIGUSR1 signal to this process */
        ExitIfBadPerror((0 == kill(g_pid_to_watch, SIGUSR1)),
                        "\nkill() is failed", errno);
    }

    /* If DNR called by user */
    if (1 == g_dnr_signal)
    {
        /* Stop the scheduler running */
        SchedulerStop(g_scheduler);

        /* Return SUCCESS status */
        return (SUCCESSED_END);
    }

    /* return REPEAT status */
    return (REPEAT);
}

static void Sigusr1Handler(int sig, siginfo_t *info, void *ucontext)
{
    /* Received SIGUSR1 signal validation */
    assert(SIGUSR1 == sig);

    /* Get the pid of signal sender */
    g_pid_to_watch = info->si_pid;

    /* Reset the time without receiving signals */
    g_time_without_sigs = 0;
    __sync_synchronize();
}

static void Sigusr2Handler(int sig, siginfo_t *info, void *ucontext)
{
    /* Received SIGUSR2 signal validation */
    assert(SIGUSR2 == sig);

    /* Turn on the DNR signal */
    g_dnr_signal = 1;
    __sync_synchronize();

    /* Send SIGUSR2 to the other process */
    ExitIfBadPerror(0 == kill(g_pid_to_watch, SIGUSR2), "\nkill() is failed",
                    errno);
}

static void CleanupResources(void)
{
    /* Clear the scheduler tasks */
    SchedulerClear(g_scheduler);

    /* Destroy the scheduler */
    SchedulerDestroy(g_scheduler);
}
