/*==============================================================================

FILE NAME:  
    uKernel.c

PURPOSE:
    uKernel RTOS implementation

FUNCTION(S):
    uKERNEL_fnCreateTask - create tasks
    uKERNEL_fnStartOS - start OS
    uKERNEL_fnDefer - defer a task
    uKERNEL_fnChangeTimeSlice - change task time slice
    uKERNEL_fnChangePri - change task priority
    uKERNEL_fnChangeState - change task state
    uKERNEL_fnKillTask - kill a task
    uKERNEL_fnKillSelf - kill task itself
    uKERNEL_fnCheckTask - check a task
    uKERNEL_fnSchedule - schedule tasks

   
    local:
        uKERNEL_fnInit  - initialize OS
        
   
Author: John Qin
Date: 6/23/2003  

    
==============================================================================*/

/*==============================================================================
                              Includes
==============================================================================*/
#include "ukernel.h"


/*==============================================================================
                              Defines
==============================================================================*/
#define TRUE    (1)

/*==============================================================================
                            Type Definitions
==============================================================================*/


/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                           Local/Private Constants
==============================================================================*/

/*==============================================================================
                           Local/Private Variables
==============================================================================*/
static uKernel_tzTaskControlBlock freeTCBList[NUM_TASKS]; // 20x#tasks
static uKernel_tzTaskControlBlock *freeTCBPtr; // 20 bytes
static uchar8 next_tid; // 1 byte
void (*uKernelCurrentFunc)(void); // 1 byte
void (*uKernelInterruptDriver)(void); // 1 byte
uint8 uKernelTicks; // current tick value 
uchar8 *uKernelTaskSP;// 1 byte
uKernel_tzTaskControlBlock *currentTask; // 20 bytes



/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

static void uKERNEL_fnInit(void);

/*==============================================================================
                           Function Definitions
==============================================================================*/




/*******************************************************************************

FUNCTION NAME: 
	uchar8 uKERNEL_fnCreateTask(void (*func)(void), 
                    uchar8 *stackBottom,
                    uchar8 stackSize,
                    uchar8 ticks)  
PURPOSE: 
    create tasks 

INPUTS: 
    *func - pointer to task function
    *stackBottom - stack bottom address
    statckSize - stack size
    ticks - cpu ticks (shares)

OUTPUTS:
    task id

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uchar8 uKERNEL_fnCreateTask(void (*func)(void), 
                    uchar8 *stackBottom,
                    uchar8 stackSize,
                    uchar8 ticks)
{
	uKernel_tzTaskControlBlock *newtsk, *temp;

	if ( freeTCBPtr == 0 )
	{
		uKERNEL_fnInit();	
	}
	newtsk = freeTCBPtr;
	freeTCBPtr = freeTCBPtr->next;
	newtsk->func = func;
	newtsk->state = T_CREATED;
	newtsk->tid = next_tid++;
	newtsk->ticks = (ticks == 0) ? DEFAULT_TICKS : ticks;

 
	// stack grows from high address to low address
	newtsk->stackBottom = stackBottom;
	newtsk->sp = (unsigned char *)(stackBottom+stackSize-1);                 	
   
    // create a circular linked list
	if ( next_tid == 1 ) {
		newtsk->next = newtsk;
		currentTask = newtsk;
	} else 
    {
		temp = currentTask->next;
		currentTask->next = newtsk;
		newtsk->next = temp;			
	}	

	return newtsk->tid;
	
}


/*******************************************************************************

FUNCTION NAME: 
	uchar8 uKERNEL_fnStartOS(void)  
PURPOSE: 
    start the multi-tasking system 

INPUTS: 
    none

OUTPUTS:
    -1: failed 
     0: success

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uchar8 uKERNEL_fnStartOS(void)
{
	if ( currentTask == 0 )
	{
		return -1;
	}
	uKERNEL_fnStartTimer();
	uKERNEL_mCTSW_ON();
	uKERNEL_fnSchedule();
	return 0; // never execute 
}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnDefer(void)
	  
PURPOSE: 
    Defer the current task 

INPUTS: 
    none

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnDefer(void)
{
	uKERNEL_mCTSW_OFF();
	uKernelTaskSP = (unsigned char *)&currentTask->sp;
	uKERNEL_fnSavregsAndResched();
}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnChangeTimeSlice(uchar8 tid, uchar8 ticks)

	  
PURPOSE: 
    Change the task's time slice, called by admin task 

INPUTS: 
    tid - task id
    ticks - task's cpu ticks

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnChangeTimeSlice(uchar8 tid, uchar8 ticks)
{
	uKernel_tzTaskControlBlock **tcblst;
	
	// search the circular list until we find a match or gone through
	// the whole list	 
	for ( tcblst = &(currentTask)->next; (*tcblst)->tid != tid && *tcblst != currentTask;
	     tcblst = &(*tcblst)->next )
	{
		;
	}
	 
	// if found, change the ticks
	if ( (*tcblst)->tid == tid )
	{
		(*tcblst)->ticks = ticks;	
	}
}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnChangePri(uchar8 tid)
	  
PURPOSE: 
    Change task priority, called by admin task 

INPUTS: 
    tid - task id

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnChangePri(uchar8 tid)
{
	uKernel_tzTaskControlBlock **tcblst, *temp1, *temp2;

	// tid == 0, no need to change admin task priority,  
	if ( tid == 0 )
	{
		return;
	}

	// search the circular list until we find a match or gone through
	// the whole list
	for ( tcblst = &(currentTask)->next; (*tcblst)->tid != tid && *tcblst != currentTask;
	      tcblst = &(*tcblst)->next )
	{
		;
	}
	// if found, then move the task to front
	if ( (*tcblst)->tid == tid )
	{
		// If this is at the front of the task list, no need to
		// change the priority
		if ( (*tcblst) == currentTask )
		{
			return;
		} 
		// move it to front 
		temp1 = *tcblst;
		*tcblst = (*tcblst)->next;  	  
		temp2 = currentTask->next;
		currentTask->next = temp1;
		temp1->next = temp2;			
				
	}

}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnChangeState(uchar8 tid, uchar8 state)

	  
PURPOSE: 
    Change a task's state 

INPUTS: 
    tid - task id
    state - task state

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnChangeState(uchar8 tid, uchar8 state)
{
	uKernel_tzTaskControlBlock **tcblst;

	// tid == 0, cannot change admin task's state,  
	if ( tid == 0 )
	{
		return;
	} 
	// search the circular list until we find a match or gone through
	// the whole list
	for ( tcblst = &(currentTask)->next; (*tcblst)->tid != tid && *tcblst != currentTask;
	     tcblst = &(*tcblst)->next )
	{
		;
	}

	// if found, then change the state
	if ( (*tcblst)->tid == tid )
	{				
		(*tcblst)->state = state;				
	}

}	

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnKillTask(uchar8 tid)
	  
PURPOSE: 
    Kill a task, called by admin task 

INPUTS: 
    tid - task id

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnKillTask(uchar8 tid)
{
	uKernel_tzTaskControlBlock **tcblst, *temp;

	// tid == 0, OS admin task, never kill this 
	if ( tid == 0 )
	{
		return;
	}

	// search the circular list until we find a match or gone through
	// the whole list
	for ( tcblst = &(currentTask)->next; (*tcblst)->tid != tid && *tcblst != currentTask;
	      tcblst = &(*tcblst)->next )
	{
		;
	}

	// if found, then delete the task
	if ( (*tcblst)->tid == tid )
	{
		// If this is the head of the task list, reassign the
		// head. This has the effect of skipping the next task
		// for one round of scheduling but should not matter
		// much.
		if ( (*tcblst) == currentTask )
		{
			currentTask = currentTask->next;
		}
 
		// delink and free the memory 
		temp = *tcblst;
		*tcblst = (*tcblst)->next;
		temp->next = freeTCBPtr;
		freeTCBPtr = temp;		
		--next_tid; 
	}

}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnKillSelf(void)
	  
PURPOSE: 
    Kill current task self 

INPUTS: 
    none

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnKillSelf(void)
{
	uKERNEL_fnKillTask(currentTask->tid);
	uKERNEL_fnSchedule();
}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnCheckTask(void)
	  
PURPOSE: 
    called by the timer interrupt. See if we need to reschedule 

INPUTS: 
    none

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/

void uKERNEL_fnCheckTask(void)
{
		
 	uKernelTicks++;	// OS timer
 	uKernelTime++; // second OS timer

	// call any user installed interrupt routine
	if ( uKernelInterruptDriver )
	{
		(*uKernelInterruptDriver)();
	}
	uKERNEL_fnRestartTimer();	

	// decrement the tick value and reschedule if needed
 
	if ( --currentTask->currentTicks == 0 )
	{

		// memorize the state, schedule another task	
		currentTask->sp = uKernelTaskSP;
		uKERNEL_fnSchedule();
			
	}


}

/*******************************************************************************

FUNCTION NAME: 
	void uKERNEL_fnSchedule(void)
	  
PURPOSE: 
     Run a different task. Set the current task as the next one in 
	 the (circular) list
	 
INPUTS: 
    none

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void uKERNEL_fnSchedule(void)
{

	// select the next task in the list
	while ( TRUE )
	{
		currentTask = currentTask->next;	
		if ( currentTask->state != T_IDLE )
		{
			break;
		} 
	}
	currentTask->currentTicks = currentTask->ticks;

	// set the global variables and go
	uKernelTaskSP = currentTask->sp;

	if ( currentTask->state == T_READY )
	{
		uKERNEL_fnResume();
	}
	else // ( task->state == T_CREATED )
	{	
		currentTask->state = T_READY;
		uKernelCurrentFunc = currentTask->func;
		uKERNEL_fnStartNewTask();
	}

	
}
	
/*******************************************************************************

FUNCTION NAME: 
	static void uKERNEL_fnInit(void)
	  
PURPOSE: 
     Initialize the RTOS
	 
INPUTS: 
    none

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
static void uKERNEL_fnInit(void)
{
	int i;

	 // initialize the TCB 
	for ( i = 0; i < NUM_TASKS-1; i++ )
	{
		freeTCBList[i].next = &freeTCBList[i+1];
	}
	// free list
	freeTCBPtr = &freeTCBList[0];
	
    uKERNEL_fnSystemInit();
}

