/*==============================================================================

FILE NAME:  
    FLASH_M.H
PURPOSE:
	flash programming
	
FUNCTION(S):
	FLASH_INIT - initialize flash programming register
    FLASH_PROTECT - setup flash programming protection
    FLASH_PROG_START - start flash programming
    FLASH_PROG_STOP - stop flash programming
    FLASH_CLOSE - close flash programming port/register

   
Author: John Qin
Date: 6/23/2003  

==============================================================================*/

#ifndef FLASH_M_H_INCL
#define FLASH_M_H_INCL


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
#define FLASH_START_ADDR  	0xBF00 //0xBF00, 0x8000, 0xBE80
#define FLASH_PROTECT_RANGE 0x7D
#define FLASH_FLCR_ADDR   0xFE08
#define FLASH_FLCR    *(volatile unsigned char *)FLASH_FLCR_ADDR
#define FLASH_FLBPR_ADDR   0xFF7E
#define FLASH_FLBPR    *(volatile unsigned char *)FLASH_FLBPR_ADDR
#define ERASE_ONE_ROW    0x30
#define HVEN			 0x03
#define MASS			 0x02
#define ERASE			 0x01
#define PGM				 0x00

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
*	FLASH PROGRAMMING MICRO
*******************************************************************/

#define FLASH_PROTECT()\
   ( REG_WRITE(&FLASH_FLBPR, FLASH_PROTECT_RANGE) ) 
#define FLASH_INIT()\
 { SET_BIT(&FLASH_FLCR,PGM);\
   PORT_READ(FLASH_FLBPR_ADDR);\
   PORT_WRITE(FLASH_START_ADDR,0xFF);}
#define FLASH_PROG_START()\
   ( SET_BIT(&FLASH_FLCR,HVEN) ) 
#define FLASH_WRITE(addr,data)\
   ( PORT_WRITE(addr,data) ) 
#define FLASH_PROG_STOP()\
   ( RESET_BIT(&FLASH_FLCR,PGM) )
#define FLASH_CLOSE()\
   ( RESET_BIT(&FLASH_FLCR,HVEN) )
   
   
#endif  // FLASH_M_H_INCL
