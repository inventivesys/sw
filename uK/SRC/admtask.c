/*==============================================================================

FILE NAME:  
    admtask.c

PURPOSE:
    OS admin task implementation

FUNCTION(S):
    uKERNEL_fnSystemInit - initialize the system
    uKERNEL_fnTimeDelay - delay time in micro seconds
    uKERNEL_fnQueryToken - query message token
    uKERNEL_fnReadToken - read from token
    uKERNEL_fnWriteToken - write to token
   
    local:
    	uKERNEL_fnAdmin_Task - OS admin task
        uKERNEL_fnOSTimeCal - calibrate the loop time
        
Author: John Qin
Date: 6/23/2003  



    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "ukernel.h"
#include "timdrv_m.h"
#include "adcdrv_m.h"
#include "pwmdrv_m.h"
#include "scidrv_m.h"
#include "flash_m.h"
#include "dipled_m.h"
#include "admtask.h"



/*==============================================================================
                              Defines
==============================================================================*/
#define UKERNEL_ADMTASK_CPUTICKS	(1)

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

static unsigned char adminTaskStack[UKERNEL_TASK_ADM_STACK_SIZE];
uKERNEL_tzSYSMsgRes sysBuffer;


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/

void uKERNEL_fnAdmin_Task(void);
void uKERNEL_fnOSTimeCal(void);

/*==============================================================================
                           Function Definitions
==============================================================================*/


/*******************************************************************************

FUNCTION NAME: 
    void uKERNEL_fnAdmin_Task(void)
  
PURPOSE: 
    OS admin task 

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
void uKERNEL_fnAdmin_Task(void)
{

	SystemStart = 0;
			
	uKERNEL_mCTSW_ON();
	while ( TRUE )
	{
	
		// __RESET_WATCHDOG();
		asm {
				sta COPCTL
		}	
			

#ifdef _USE_TOKEN
		uKERNEL_fnQueryToken(1, 1);

		if (  PTF_PTF0 )
		{
			uKERNEL_mCTSW_OFF();		
			uKERNEL_fnCreateTask(SCISVC_fnTask, (unsigned char *)&SCISVC_TaskStack[0],
			(unsigned char)UKERNEL_TASK_DF_STACK_SIZE, 1);		
			uKERNEL_mCTSW_ON();				
			
		}		
		if ( PTE & 0x04 )
		{
			uKERNEL_mCTSW_OFF();
			uKERNEL_fnCreateTask(SSSVC_fnTask, (unsigned char *)&SSSVC_TaskStack[0],
			(unsigned char)UKERNEL_TASK_DF_STACK_SIZE, 1);		
			uKERNEL_mCTSW_ON();			
		}		
	
	
				
#endif			

	}
}

/*******************************************************************************

FUNCTION NAME: 
    void uKERNEL_fnSystemInit( void )
  
PURPOSE: 
    create OS admin task which admin system reources

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
void uKERNEL_fnSystemInit( void )
{


	uKERNEL_fnCreateTask(uKERNEL_fnAdmin_Task, (unsigned char *)&adminTaskStack[0],
		(unsigned char)UKERNEL_TASK_ADM_STACK_SIZE, UKERNEL_ADMTASK_CPUTICKS);
	

#ifdef _USE_TOKEN 			
	sysBuffer.status = 0;				
	sysBuffer.des = 0;	
	sysBuffer.currentPri = 0;
	sysBuffer.resPri = 0;	
	uKERNEL_InitTimer();	
#endif

#ifdef _ACTIME
	uKERNEL_mCTSW_ON();
	uKERNEL_fnOSTimeCal();
	uKERNEL_mCTSW_ON();
#endif //_ACTIME

	DIPLEDDRV_mINIT();
		
}

/*******************************************************************************

FUNCTION NAME: 
    void uKERNEL_fnOSTimeCal( void )
  
PURPOSE: 
    calibrate a loop time 

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
void uKERNEL_fnOSTimeCal( void )
{
    uint16 counterValue1 = 0; 
    uchar8 i;
    uint8 aLoopTime;

    counterValue1 = TMRDRV_mTIMER_READ(0);//read timer A
    for (i=0; i<10; i++)
    {
        ;
    }
    asm {//debug
    	lda TACNTL
    }     
    counterValue1 = TMRDRV_mTIMER_READ(0)-counterValue1;
    /*a FOR loop time in u seconds = aLoopClockCycles * Clock period*/  
    aLoopTime =(FQ_OPCLK/(0x01 << (TMRDRV_mTIMER_READ_STATUS(0) & TIMER_PRESCAL_MASK)) );
	aLoopTime = 10000/(aLoopTime/10);//u Secoond
    if ( aLoopTime == 0 )
    {
    	 aLoopTime = 1;//in case it rounds out to 0
    }	
}

/*******************************************************************************

FUNCTION NAME: 
    uchar8 uKERNEL_fnTimeDelay(uint8 delayTime)
  
PURPOSE: 
    delay time in micro (u) seconds

INPUTS: 
    delayTime: delay time

OUTPUTS:
    0: sucess
    1: error

NOTES:
    delay_time = (12+(5+(11+72+11)*A +11)+8)/bus_fq

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.

*******************************************************************************/

uchar8 uKERNEL_fnTimeDelay(uint8 delayTime)
{
    uchar8 loop;

    if ( delayTime > 10000 )
    {
        return 1;// reject it as it may choose to suspend the task
    }
#ifdef _ACTIME

	if (aLoopTime != 0)
   	{
    		for (loop = 0; loop < (delayTime/aLoopTime)+1; loop++)
    		{
        		;       
        	}      
		    return 0;
	} else
	{
			return 2;//error return
	}
#endif //_ACTIME
	while ( delayTime !=0 ) 
	{
 		for (loop=0x03;loop!=0;loop--);
 		delayTime--;
	}
	
	return 0;


}

/*******************************************************************************

FUNCTION NAME: 
    uchar8 uKERNEL_fnQueryToken(uchar8 token, uchar8 pri)

  
PURPOSE: 
    Query message token 

INPUTS: 
    token: token that is interested
    pri: task priority

OUTPUTS:
    token's status

NOTES:
    <notes>

VERSION HISTORY:


Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    
*******************************************************************************/
uchar8 uKERNEL_fnQueryToken(uchar8 token, uchar8 pri)
{
	
	switch (token) {
		case 1:
			{
				if ( sysBuffer.status == 1 )
				{
					if ( sysBuffer.resPri < pri )
					{
						sysBuffer.resPri = pri;
					}
				} else
				{
					sysBuffer.currentPri = pri;
					sysBuffer.status = 1;
				}
			}
		break;
		default:
		break;
	}
	
	return sysBuffer.resPri;
	
}

/*******************************************************************************

FUNCTION NAME: 
    uint8 uKERNEL_fnReadToken(uchar8 token)
  
PURPOSE: 
    read data from token's receive queue 

INPUTS: 
    token: token that task is holding

OUTPUTS:
    the data in token's receive queue

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.

*******************************************************************************/
uint8 uKERNEL_fnReadToken(uchar8 token)
{

	
	return uKERNEL_mRB_GETBYTE(sysBuffer.sysRXBuf);
	
}

/*******************************************************************************

FUNCTION NAME: 
    void uKERNEL_fnWriteToken(uchar8 token, uchar8 dataByte)
  
PURPOSE: 
    Write meg to token transmit queue 

INPUTS: 
    token: token that the task holds
    dataByte: data to transmit

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.

*******************************************************************************/
void uKERNEL_fnWriteToken(uchar8 token, uchar8 dataByte)
{

	
        if ( !uKERNEL_mRBUF_FULL(sysBuffer.sysTXBuf,RB_BUF_SIZE2) )	              
        {
              uKERNEL_mRB_PUTBYTE(sysBuffer.sysTXBuf, dataByte);           	
        } else
    	{
    	 	  uKERNEL_mRESET_RBUF(sysBuffer.sysTXBuf);
    	      uKERNEL_mRB_PUTBYTE(sysBuffer.sysTXBuf, LO_BYTE(dataByte));     	 	  	  
    	}
	
}