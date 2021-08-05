/*==============================================================================

FILE NAME:  
    DIPLED_M.H

PURPOSE:
	dip-swich and led I/O ports driver
	
FUNCTION(S):

    DIPLEDDRV_mINIT - initialize dip-switch and led ports
	DIPLEDDRV_mCLOSE - close dip-switch and led ports
	   
   
Author: John Qin
Date: 6/23/2003  


==============================================================================*/

#ifndef DIPLED_H_INCL
#define DIPLED_H_INCL


/*==============================================================================
                              Includes
==============================================================================*/
#include <MC68HC908MR16.h> 
#include "HC908MRIO.h"
#include "LIB_stdtype.h"    // Requires and architecture defn. in the compiler
#include "LIB_stddefs.h"    // Standard definitions for all projects
#include "uKutil.h"

/*==============================================================================
                              Defines
==============================================================================*/
#define LED_FLASH_FAST	(10)
#define LED_FLASH_SLOW  (100)
/*==============================================================================
                            Type Definitions
==============================================================================*/

/*=============================================================================
                                Enums
==============================================================================*/

/*==============================================================================
                              Structures
==============================================================================*/

/*==============================================================================
                             Constants
==============================================================================*/
 
/*==============================================================================
                               Variables
==============================================================================*/


/*==============================================================================
                            Function Protoypes/MICROS
==============================================================================*/

/******************************************************************
*	DIP SWITCH/LED MICRO
*******************************************************************/

//PTA1-6: LED1-6
//PTC3: LED 7
//PTE0: LED8
#define DIPLEDDRV_mINIT()\
 { DDRA = 0x7E;\
   PTA = 0x00;\
   DDRC = 0x08;\
   PTC = 0x00;\
   PTD = 0x00;\
   DDRE = 0x01;\
   PTE = 0x00;\
   DDRF = 0x00;\
   PTF = 0x00;}
#define DIPLEDDRV_mCLOSE()\
  { PTA = 0x00;\
    PTC = 0x00;\
    PTE = 0x00; }
   
   
#endif  // DIPLED_H_INCL
