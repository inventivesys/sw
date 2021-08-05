/*==============================================================================

FILE NAME:  
    pwmdrv_m.h

PURPOSE:
    pwm driver

FUNCTION(S):
    PWMDRV_mINIT - initialize pwm
    PWMDRV_mSTART - start pwm
    PWMDRV_mCTL - pwm control
    PWMDRV_mSET_PRESCALE - set pwm prescale
    PWMDRV_mSET_PRD - pwm set period
    PWMDRV_mREAD - read pwm counter
    PWMDRV_mSTA_READ - read pwm status
    PWMDRV_mINT_EN_DIS - pwm interrupt enable/disable
    PWMDRV_mDUTYCYCLE_ADJUST - pwm duty adjustment %
    PWMDRV_mVAL - pwm duty value adjustment
    PWMDRV_mPIN_SELECT - pwm pin selection
    PWMDRV_mPIN_CTL - pwm pin control
    PWMDRV_mFAULT_ENABLE - pwm fault pin enable
    PWMDRV_mFAULT_DISABLE - pwm fault pin disable
    PWMDRV_mFAULT_ACK - pwm fault ackonwledgement
    PWMDRV_mSTOP - stop pwm
    
Author: John Qin
Date: 6/23/2003  
    
==============================================================================*/
#ifndef PWMDRV_M_H_INCL
#define PWMDRV_M_H_INCL
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
#define PWM_DF_CTL1     0x02//bank x & y enabled, prescale loaded, PWM stopted
#define PWM_DF_CTL2     0x00//default setup:PWM reload every PWM cycle,clock fq=f(op)
                            //f(op)=1MHz used, fq=1MHz, 1 uS for each cycle
#define PWM_DF_CTLMOD   0x07//edge aligend mask
#define PWM_DF_DEADTM   0xFF//6 independent channels, no dead time insertion
#define PWM_DF_DISMAP   0x00//no pin disable when fault occurs on pin 1 or 2
#define PWM_DF_FAULT    0x00//no fault pin enabled (pin1-0x02, pin2-0x0C)
#define PWM_START_MASK  0x03
#define PWM_INT_MASK    0x00

#define FAULT_PIN1      0x01
#define FAULT_PIN2      0x03
#define FAULT_ACK1      0x00
#define FAULT_ACK2      0x02
#define FAULT_SYS_DF	0x02

#define PWM_FQ      	3900
#define PWM_6CHN		0x3F



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
 *PWM initialization, both bank x & y enabled
 *6 independent channels, edge aligend, 
 *fault intr enabled, PWM stoped
 */
#define PWMDRV_mINIT()\
 { REG_WRITE(PWM_DEADTM_ADDR,PWM_DF_DEADTM);\
   REG_WRITE(PWM_DISMAP_ADDR,PWM_DF_DISMAP);\
   REG_WRITE(&PWM_FAULT,PWM_DF_FAULT);}
/*start PWM */      
#define PWMDRV_mSTART()\
  { PCTL1 |= MASK_BIT(0);\
    PCTL1 |= MASK_BIT(1);}
/*PWM control, reg 1-CTL1, reg 2-CTL2*/      
#define PWMDRV_mCTL(reg, ctrl)\
   (reg-1? REG_WRITE(PWM_CTL2_ADDR, ctrl):REG_WRITE(PWM_CTL1_ADDR, ctrl) )
/*manually control 6 PWM pins, 1-active, 0-inactive. e.g. B01000001-pin 1 active*/
#define PWMDRV_mPIN_CTL(ctrl)\
   ( REG_WRITE(&PWM_OUT_CTL, ctrl) )   
/* manually control 6 PWM pins, 1-active, 0-inactive
 *e.g. B01000001-pin 1 active. toggle:0-inactive, 1-active */
#define PWMDRV_mPIN_SELECT(pin, toggle)\
   (toggle? PWM_OUT_CTL |= MASK_BIT(pin):PWM_OUT_CTL &= ~MASK_BIT(pin))
/*PWM fault pin select, auto mode, either pin 1-0x02 or pin 2-0x0c*/      
#define PWMDRV_mFAULT_ENABLE(pin)\
   ((pin-1)? (PWM_FAULT |= MASK_BIT(FAULT_PIN2)):(PWM_FAULT |= MASK_BIT(FAULT_PIN1)))//(REG_WRITE(PWM_FAULT_CTL_ADDR, FAULT_SYS_DF)))
/*PWM fault de-select, either pin 1 or 2*/      
#define PWMDRV_mFAULT_DISABLE(pin)\
   ((pin-1)?(PWM_FAULT &= ~MASK_BIT(FAULT_PIN2)):(PWM_FAULT &= ~MASK_BIT(FAULT_PIN1)))  
/*PWM fault ack, either pin 1 or 2*/      
#define PWMDRV_mFAULT_ACK(pin)\
   ((pin-1)?(PWM_FAULT_ACK |= MASK_BIT(FAULT_ACK2)):(PWM_FAULT_ACK  |= MASK_BIT(FAULT_ACK1)))     
/*PWM fq setup*/
#define PWMDRV_mSET_PRESCALE(scale)\
    PWM_CTL2 |= MASK_BIT(scale)
/*PWM period setup,in edge aligend,for default setting, max modules is 4095*/
#define PWMDRV_mSET_PRD(prd)\
    PWM_MOD = FQ_OPCLK/(prd*(PWM_CTL2 & 0x03))
/*PWM counter read*/
#define PWMDRV_mREAD\
    (PWM_CNT)
/*PWM status read*/
#define PWMDRV_mSTA_READ() (FSR)				
/*PWM interrupt enable/disable. toggle 1 for enable, 0 for disable*/		
#define PWMDRV_mINT_EN_DIS(toggle)\
	(toggle ? (PCTL1 |= MASK_BIT(PWM_INT_MASK)):(PCTL1 &= ~MASK_BIT(PWM_INT_MASK)))
/*PWM duty cycle adjustment*/ 
#define PWMDRV_mDUTYCYCLE_ADJUST(chn, percent)\
    PWM_VALUE(chn) = percent*(PWM_MOD+1)/100-1
/*PWM duty cycle adjustment*/ 
#define PWMDRV_mVAL(chn, val)\
   ( PWM_VALUE(chn) = val )
/*stop PWM on all channels*/		
#define PWMDRV_mSTOP()\
    PWM_CTL1 &= ~MASK_BIT(PWM_START_MASK)		
		
#endif //PWMDRV_M_H_INCL     
   
