#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>	/* size_t */
#include <time.h>	/* time_t */

#include "uid.h"	/* u_id_t */

typedef struct task task_t;

typedef enum op_f_status {
						  REPEAT = -1, 
						  SUCCESSED_END = 0, 
						  FAILED_END = 1
						 } op_f_status;

typedef op_f_status (*oper_func_t)(void *param);


/*****************************  Task FUNCTIONS *******************************/

/*******************************************************************************
* Function:          Create a task
* Arguments:         op_func  - pointer to function.
                     param    - pointer to the function arguments.
                     run_time - the first time to run.
                     interval - repeats again in the next time.
* Return value:      task_t pointer to the new task
* Notes: 	         If func is NULL - undefined bevhavior.  
* Time Complexity:   O(1)
*******************************************************************************/
task_t *TaskCreate(oper_func_t func, void *param, time_t run_time, 
                   size_t interval);
 
/*******************************************************************************
* Function:         Destroy the task. 
* Arguments:        task - pointer to the task to destroy.
* Return value:     void.
* Notes: 	        If task is NULL - no operation is performed.
* Time Complexity:  O(1)
*******************************************************************************/
void TaskDestroy(task_t *task);

/*******************************************************************************
* Function:         Returns the uid of task.    
* Arguments:        task   - pointer to task.
* Return value:     ui_d_t - uid of task.
* Notes: 	        If task is NULL  - undefined bevhavior.
* Time Complexity:	O(1).
*******************************************************************************/
u_id_t TaskGetUid(task_t *task);

/*******************************************************************************
* Function: 	    Returns the run time of task.                 
* Arguments:  	    task   - pointer to task.
* Return value:     time_t - the run time of task.
* Notes:			If task is NULL  - undefined bevhavior.
* Time Complexity:	O(1).
*******************************************************************************/
time_t TaskGetRunTime(task_t *task);

/*******************************************************************************
* Function: 	    Update the run time of task according to the interval time.                 
* Arguments:  	    task   - pointer to task.
* Return value:     
* Notes:			If task is NULL  - undefined bevhavior.
* Time Complexity:	O(1).
*******************************************************************************/
void TaskUpdateRunTime(task_t *task);

/*******************************************************************************
* Function: 	    run the task's function.                 
* Arguments:  	    task   - pointer to task.
* Return value:     In case of succeeded finish          - SUCCESS.
                    In case of failed finish             - FAIL.
                    In case of success but didn't finish - REPEAT.
* Notes:			If task is NULL                      - undefined bevhavior.
* Time Complexity:	O(1).
*******************************************************************************/
op_f_status TaskRunFunction(task_t *task);


#endif /* __TASK_H__ */
