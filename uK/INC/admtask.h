/*==============================================================================

FILE NAME:  
    admtask.h

PURPOSE:
    OS admin task

FUNCTION(S):
    uKERNEL_fnSystemInit - initialize the system
    uKERNEL_fnTimeDelay - delay time in micro seconds
    uKERNEL_fnQueryToken - query message token
    uKERNEL_fnReadToken - read from token
    uKERNEL_fnWriteToken - write to token
   
   
NOTES:
    #$-VIEW:C Coding Standard-$#
    **If you do not see a button above this, either use CodeWright or ignore 
    **the text.
    

Author: John Qin
Date: 6/23/2003  

    
==============================================================================*/
#ifndef _uADMTASK_H
#define _uADMTASK_H


/*==============================================================================
                              Includes
==============================================================================*/


/*==============================================================================
                              Defines
==============================================================================*/

#define SYS_ADM_PRI		 (0x10)
#define SYS_PWM_PRI		 (0x09)
#define SYS_ADC_PRI 	 (0x08)
#define SYS_LED_PRI		 (0x07)
#define SYS_SCI_PRI		 (0x06)
#define SYS_ISHUNT_PRI	 (0x05)
/*==============================================================================
                            Type Definitions
==============================================================================*/

typedef struct zSysMsgRes {
	uchar8 status;
    uchar8 des;
    uchar8 currentPri;    
    uchar8 resPri;
    uKERNEL_tzRingBuf sysTXBuf;
    uKERNEL_tzRingBuf sysRXBuf;  
} uKERNEL_tzSYSMsgRes; 



/*==============================================================================
                                Enums
==============================================================================*/


/*=============================================================================
                              Structures
==============================================================================*/


/*==============================================================================
                          External/Public Constants
==============================================================================*/


/*==============================================================================
                          External/Public Variables
==============================================================================*/
extern uchar8 SystemStart;

/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
void uKERNEL_fnSystemInit();
uchar8 uKERNEL_fnTimeDelay(uint8 delayTime);
uchar8 uKERNEL_fnQueryToken(uchar8 token, uchar8 pri);
void uKERNEL_fnWriteToken(uchar8 token, uchar8 dataByte);


#endif //_uADMTASK_H
