#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>     /* size_t */

#include "task.h"       /* oper_func_t, time_t, u_id_t */

typedef struct scheduler scheduler_t;
typedef enum exec_status {SUCCESS_EXEC = 0, FAIL_EXEC = 1} exec_status;

/**************************  Scheduler FUNCTIONS ******************************/
/*******************************************************************************
* Function:          Creates a new scheduler.
* Arguments:            
* Return value:      scheduler_t * - Pointer to the new scheduler.
* Notes:             
* Time Complexity:   O(1)
*******************************************************************************/
scheduler_t *SchedulerCreate(void);

/*******************************************************************************
* Function:            Destroy the scheduler.
* Arguments:           sched - Pointer to the scheduler.
* Return value:        
* Notes:               If sched is NULL - no operation is performed.
* Time Complexity:     O(n)
*******************************************************************************/
void SchedulerDestroy(scheduler_t *sched);
 
/*******************************************************************************
* Function:            Function adds a new task to the scheduler.              
* Arguments:           sched    - pointer to scheduler.
                       op_func  - user operational func.
                       param    - pointer to user parameter.
                       run_time - function run time.
                       interval - time interval between function runs.
* Return value:        u_id_t - the new task uid.  
* Notes:               In fail case returns ERROR UID.
                       If sched is NULL   - undefined behavior.
                       If op_func is NULL - undefined behavior.
* Time Complexity:     O(n)
*******************************************************************************/
u_id_t SchedulerAdd(scheduler_t *sched, oper_func_t op_func, void* param,  
                    time_t run_time, size_t interval);

/*******************************************************************************
* Function:            Removes a task from the scheduler
* Arguments:           uid - the uid of the task.
                       sched - pointer to scheduler.
* Return value:        status - '0' for success and '1' for fail execution.
* Notes:               If sched is empty - no operation be preformed.
                       If sched is NULL  - undefined behavior.
* Time Complexity:     O(n)
*******************************************************************************/
exec_status SchedulerRemove(scheduler_t *sched, u_id_t uid);

/*******************************************************************************
* Function:            Run the scheduler.
* Arguments:           sched - pointer to scheduler.
* Return value:        status - '0' for success and '1' for fail execution.
* Notes:               If the sched is empty - no operation is performed.
                       If sched is NULL      - undefined behavior.
* Time Complexity:     O(1).
*******************************************************************************/
exec_status SchedulerRun(scheduler_t *sched);

/*******************************************************************************
* Function:            Stop the scheduler.
* Arguments:           sched - pointer to scheduler.
* Return value:        status - '0' for success. '1' for fail.
* Notes:               If the sched is empty - no operation is performed.
                       If sched is NULL      - undefined behavior.
                       It is recommended to use SchedulerStop as part of 
                       the op_func you are passing to SchedulerAdd. 
* Time Complexity:     O(1).
*******************************************************************************/
void SchedulerStop(scheduler_t *sched);

/*******************************************************************************
* Function:            Counts the number of tasks in scheduler.
* Arguments:           sched - pointer to scheduler.                
* Return value:        number of tasks.
* Notes:               If sched is NULL - undefined behavior.
* Time Complexity:     O(n).
*******************************************************************************/
size_t SchedulerSize(scheduler_t *sched);

/*******************************************************************************
* Function:            Checks if the scheduler is empty.
* Arguments:           sched - pointer to scheduler.
* Return value:        '0' for not empty, '1' for empty.
* Notes:               If sched is NULL - undefined behavior.
* Time Complexity:     O(1).
*******************************************************************************/
int SchedulerIsEmpty(scheduler_t *sched);

/*******************************************************************************
* Function:            Remove all scheduler’s tasks.
* Arguments:           sched - pointer to scheduler.
* Return value:        
* Notes:               If the sched is empty - no operation is performed.
                       If sched is NULL - undefined behavior.
* Time Complexity:     O(n).
*******************************************************************************/
void SchedulerClear(scheduler_t *sched);


#endif  /* __SCHEDULER_H__ */
