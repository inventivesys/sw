/*==============================================================================

FILE NAME:  
    adcdrv_m.h

PURPOSE:
    a/d converter driver

FUNCTION(S):
    ADCDRV_mADC_INIT - initialize ADC port
	ADCDRV_mADC_CHN_OPEN - open ADC channel
	ADCDRV_mADC_CHN_CLOSE - close ADC channel
	ADCDRV_mADC_READHI - read ADC high byte data
	ADCDRV_mADC_READLO - read ADC low byte data
	ADCDRV_mADC_CTL - ADC control
	ADCDRV_mADC_CLOCK_CTL - ADC clock control
	ADCDRV_mADC_INTR_EN_DIS - ADC interrupt enable/disable
	ADCDRV_mADC_CLOSE - close ADC port
	
Author: John Qin
Date: 6/23/2003  
    
==============================================================================*/
#ifndef ADCDRV_M_H_INCL
#define ADCDRV_M_H_INCL
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
#define ADC_DF_CTL	 	(0x00) //initial setup, one conversion, intr disabled
#define ADC_OFF 	 	(0x1F) //adc powered off
#define ADC_DF_CLK	 	(0x04) //clock src (4/4)MHz, ratio 1, 10-bit right justified
#define ADC_AIEN	 	(0x06) //interrut mask
#define ADC_START    	(0x40) 
#define ADC_MAX_CHN_NO  (10)



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
/*
 *initial setup, one conversion, intr disabled
 *right justifed mode, external clk
 */
#define ADCDRV_mADC_INIT()\
	{ REG_WRITE(&AD_CTL, ADC_DF_CTL);\
      REG_WRITE(&AD_CLK, ADC_DF_CLK);}
/*select and open this channel (1-10); before open, you need to close previous channel*/      
#define ADCDRV_mADC_CHN_OPEN(chn) ( REG_WRITE(&AD_CTL, ADC_DF_CTL|(chn-1)) )
/*close channel before open a new one*/      
#define ADCDRV_mADC_CHN_CLOSE(chn) ( REG_WRITE(&AD_CTL, 0x00) )      
/*read ADC value, 10-bit, right justifed*/
#define ADCDRV_mADC_READHI(chn) ( AD_DATAHI )
#define ADCDRV_mADC_READLO(chn) ( AD_DATALO )
/*adc control*/
#define ADCDRV_mADC_CTL(chn, ctrl) ( REG_WRITE(&AD_CTL, ctrl|chn) )
/*adc clock control*/
#define ADCDRV_mADC_CLOCK_CTL(ctrl) ( REG_WRITE(&AD_CLK, ctrl) )
/*close all adc channels, power off adc*/
#define ADCDRV_mADC_CLOSE   ( REG_WRITE(&AD_CTL, ADC_OFF) )
/*ADC interrupt enable/disable. toggle 1 for enable, 0 for disable*/		
#define ADCDRV_mADC_INTR_EN_DIS(toggle)\
  (toggle ? (AD_CTL |= MASK_BIT(ADC_AIEN)) : (AD_CTL &= ~MASK_BIT(ADC_AIEN)))


#endif //ADCDRV_M_H_INCL     
   
