/*==============================================================================

FILE NAME:  
    timdrv_m.h

PURPOSE:
    timer driver

FUNCTION(S):
    TMRDRV_mTIMER_INIT - initialization
    TMRDRV_mTIMER_START - start timer
    TMRDRV_mTIMER_CTL - timer control
    TMRDRV_mTIMER_READ - read timer counter
    TMRDRV_mTIMER_READ_STATUS - read timer status
	TMRDRV_mTIMER_SET_PRD - set timer period
	TMRDRV_mTIMER_CLEAR - clear timer counter
	TMRDRV_mTIMER_HALT - halt timer
	TMRDRV_mTIMER_RESUME - resume timer
	TMRDRV_mTIMER_RESET - reset timer
	TMRDRV_mTIMER_STOP - stop timer
	TMRDRV_mTMRCHN_INTR_EN - enable/disable timer channel interrupt
	TMRDRV_mTMRCHN_CLEARINT - clear timer channel interrupt		
   
Author: John Qin
Date: 6/23/2003  

==============================================================================*/
#ifndef TIMDRV_M_H_INCL
#define TIMDRV_M_H_INCL
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
#define TOE			 0x07
#define TOIE		 0x06
#define TSTOP		 0x05
#define TRST		 0x04
#define CHXIE		 0x06
#define CHXF		 0x07
#define CHSTOP       0x00

#define TM_DF_CTRL	 0x36 //cleared and stoped, clicks once every  1(0x30) or 64(0x36) BUS cycles
#define TM_DF_PRD    0xFFFF //max value
#define TM_DF_PSCAL	 0x00 //(4/4)MHz clock, ratio 1, clk period 1us, overflow period 65.536ms
#define TM_DF_CLKPD	 0x01 //clk period 1us
#define TM_CH_INIT   0x54 //output compare, intr enabled, no pin control
#define TIMER_PRESCAL_MASK  0x07



#define ONESECONDTICK  100 //32, 92
#define TWOSECONDTICK  200 //32, 92
#define ONEMINUTETICK  5520
#define FIVESECONDTICK 460
#define TENSECONDTICK  920
#define TWENTYSECONDTICK 1840
#define MS20		   2
#define MS30		   3
#define MS40		   4
#define MS50		   5
#define MS60		   6
#define MS100		   10
#define MS200		   20
#define MS300		   30
#define MS400		   40
#define MS500		   50
#define MS600		   60
#define MS800		   80
#define MS900		   90
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
/*initial setup to be cleared and stoped, clicks once every 64 BUS cycles*/
#define TMRDRV_mTIMER_INIT(port)\
       { TM_CTL(port) = TM_DF_CTRL;\
         TM_PRD(port) = TM_DF_PRD; }
/*start the timer*/         	         
#define TMRDRV_mTIMER_START(port)\
        TM_CTL(port) &= ~MASK_BIT(TSTOP)
/*timer control*/                                
#define TMRDRV_mTIMER_CTL(port, ctrl)\
       { TM_CTL(port) = ctrl;}       
/*read timer counter value*/       
#define TMRDRV_mTIMER_READ(port)\
		TM_CNT(port)  
/*read timer status*/   
#define TMRDRV_mTIMER_READ_STATUS(port)\
        TM_CTL(port)
/*setup timer period*/        	
#define TMRDRV_mTIMER_SET_PRD(port, prd)\
		TM_PRD(port) = prd         
/*clear timer couter*/		
#define TMRDRV_mTIMER_CLEAR(port)\
        TM_CTL(port) &= ~MASK_BIT(TOE)        		
/*halt the timer*/        
#define TMRDRV_mTIMER_HALT(port)\
        TM_CTL(port) |= MASK_BIT(TSTOP)	 
/*resume the timer after a halt*/        
#define TMRDRV_mTIMER_RESUME(port)\
        TM_CTL(port) &= ~MASK_BIT(TSTOP)					
/*reset the timer, this clears couter too*/        
#define TMRDRV_mTIMER_RESET(port)\
		TM_CTL(port)  |= MASK_BIT(TRST)	
/*stop the timer*/		
#define TMRDRV_mTIMER_STOP(port)\
        { TM_CTL(port)  |= MASK_BIT(TRST);\
          TM_CTL(port) |= MASK_BIT(TSTOP);}
        
/*initial setup to output compare, intr enabled, no pin control*/
#define TMRDRV_mTMRCHN_INIT(port, chn)\
       { TM_CHNCTL(port, chn) = TM_CH_INIT;\
         TM_LATCH(port, chn) = TM_DF_PRD; }
/*set up channel period*/
#define TMRDRV_mTMRCHN_SET_PRD(port, chn, prd)\
		 TM_LATCH(port, chn) = prd	
/*read channel latch value*/            
#define TMRDRV_mTMRCHN_READ(port, chn)\
         TM_LATCH(port, chn)         
/*channel control*/         
#define TMRDRV_mTMRCHN_CTL(port, chn, ctrl)\
       { TM_CHNCTL(port, chn) = ctrl;}	 
/*stop timing activity(output compare/input capture) on this channel*/
#define TMRDRV_mTMRCHN_STOP(port, chn)\
       { TM_CHNCTL(port, chn) = CHSTOP;}  
#define TMRDRV_mTMRCHN_CLEARINT(port, chn)\
       { TM_CHNCTL(port,chn) &= ~MASK_BIT(CHXIE);\
       	 TM_CHNCTL(port,chn);\
       	 TM_CHNCTL(port,chn) &= ~MASK_BIT(CHXF);} 
/*timer channel interrupt enable/disable, toggle 1-enable, 0-disable*/                    	
#define TMRDRV_mTMRCHN_INTR_EN(port, chn, toggle)\
  (toggle? (TM_CHNCTL(port,chn) |= MASK_BIT(CHXIE)):(TM_CHNCTL(port,chn) &= ~MASK_BIT(CHXIE)) ) 
		
     
#endif //TIMDRV_M_H_INCL     
   
