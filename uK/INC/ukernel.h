/*==============================================================================

FILE NAME:  
    ukernel.h

PURPOSE:
    OS 

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

   
Author: John Qin
Date: 6/23/2003  
    
==============================================================================*/

#ifndef _uKERNEL_H
#define _uKERNEL_H

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // Requires and architecture defn. in the compiler

/*==============================================================================
                              Defines
==============================================================================*/

#undef _DEBUG_ASM
#undef _DEBUG_FLASH_PROG
#define _DEBUG_PC_SCI



#ifndef NUM_TASKS
#define NUM_TASKS		8
#endif //NUM_TASKS

#define INTERRUPT_ON    1
#define INTERRUPT_OFF   0

// Minimum stack size for a task that does not use "any" sys stack space.
// This should be the interrupt stack frame, plus some fudge factor for
// frame pointer, a single function call, etc.
#define UKERNEL_TASK_DF_STACK_SIZE	(26)
#define UKERNEL_TASK_ADM_STACK_SIZE	(26)
#define DEFAULT_TICKS				(1)

/*=============================================================================
                                Enums
==============================================================================*/
enum { T_CREATED, T_READY, T_RUNNING, T_IDLE };

/*==============================================================================
                            Type Definitions
==============================================================================*/

typedef struct zTaskControlBlock
{
	struct	TaskControlBlock *next;
	uchar8 tid;			/* task id */
	uchar8 state;		/* task state, do not use enum
								 * since the compiler may allocate
								 * more space 
							     */
	uchar8 ticks;		/* how many ticks does the task execute */
	uchar8 currentTicks;/* current number of ticks remaining */
	void	(*func)(void);		/* function to call for that task */
	uchar8	*stackBottom;/* stack low value */
	uchar8	*sp;		/* current value of the stack pointer */
} uKernel_tzTaskControlBlock;


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/

extern uint8 uKernelTicks; // current tick value
extern uint8 uKernelTime;  // current tick value
extern uchar8 *uKernelTaskSP;	// stack pointer
extern uKernel_tzTaskControlBlock *currentTask; //20 bytes


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/

/*==============================================================================
                            Function Protoypes/MICROS
==============================================================================*/

#ifndef uKERNEL_mCTSW_ON
#define uKERNEL_mCTSW_ON()	{__asm cli;}
#define uKERNEL_mCTSW_OFF()	{__asm sei;}
#define CLEAR_COP()	{__asm STA COPCTL;}
#endif

// 4000 ticks/sec for 8Mhz clock 2000 cycles/tick 
uchar8 uKERNEL_fnCreateTask(void (*func)(void), 
                    uchar8 *stackBottom, 
                    uchar8 stackSize, 
                    uchar8 ticks);

void uKERNEL_fnDefer(void);
void uKERNEL_fnChangeTimeSlice(uchar8 tid, uchar8 ticks);
void uKERNEL_fnKillTask(uchar8 pid);
void uKERNEL_fnSchedule(void);
void uKERNEL_fnResume(void);
void uKERNEL_fnKillSelf(void);
void uKERNEL_SystemInterrupt(void);
void uKERNEL_fnStartTimer(void);
void uKERNEL_fnRestartTimer(void);
void uKERNEL_fnSavregsAndResched(void);
void uKERNEL_fnStartNewTask(void);
void uKERNEL_fnChangePri(uchar8 tid);
void uKERNEL_fnChangeState(uchar8 tid, uchar8 state);
uchar8 uKERNEL_fnStartOS(void);
void uKERNEL_SystemInterrupt(void);
extern void (*uKernelInterruptDriver)(void);





#endif //_uKERNEL_H
