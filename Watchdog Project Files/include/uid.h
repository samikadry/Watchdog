#ifndef __UID_H__
#define __UID_H__

#include <time.h>			/* size_t, time */
#include <sys/types.h>		/* pid_t */

typedef struct uid
{
	size_t count;	/* counter of objects in process */
	time_t time;	/* current time */
    pid_t pid;     	/* pid_t is signed int */  
 	
}u_id_t;

/******************************* USER FUNCTIOS ********************************/

/*******************************************************************************
* Function:         creat new Uid   			
* Arguments:         			
* Return value:     u_id_t - the new uid
* Notes: 	        
* Time Complexity:	O(1)
*******************************************************************************/
u_id_t UidCreate(void);

/*******************************************************************************
* Function:         Check if two uid's are Equal.       			
* Arguments:        uid1 - id to object 1    
                    uid2 - id to object 2      			
* Return value:     (1)  - if equal
                    (0)  - if NOT equal.
* Notes: 	        
* Time Complexity:   O(1)	
*******************************************************************************/
int UidsIsEquals(const u_id_t uid1, const u_id_t uid2);

/*******************************************************************************
* Function : 		create an  ERRUID 			
* Arguments:        		
* Return value:     u_id_t - ERROR uid 
                   
* Notes: 	        
* Time Complexity:	O(1)
*******************************************************************************/
u_id_t ErrUidCreate(void);

#endif /* __UID_H__ */
