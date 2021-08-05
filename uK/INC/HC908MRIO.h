/*==============================================================================
FILE NAME:  
    HC908MRIO.H
PURPOSE:
	IO DEFINITIONS FOR MC68HC908MR16, MC68HC08AZ32
FUNCTION(S):
   
Author: John Qin
Date: 6/23/2003  

==============================================================================*/

#ifndef HC908MRIO_H_INCL
#define HC908MRIO_H_INCL

#define _8BIT_ARCHITECTURE
/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // Requires and architecture defn. in the compiler
#include "LIB_stddefs.h"    // Standard definitions for all projects


/*******************************************************************/
/* TIMER REGISTER MICROS  (TIM0 = Timer A, TIM1 = Timer B          */                                       
/*******************************************************************/
#define TM_ADDR(port) (port ? 0x52:0x0f)
#define TM_CNT(port) *(volatile unsigned int*)TM_ADDR(port)
#define TM_PRD_ADDR(port) (port ? 0x54 :0x11)
#define TM_PRD(port) *(volatile unsigned int*)TM_PRD_ADDR(port)
#define TM_CTL_ADDR(port) (port ? 0x51 : 0x0e )
#define TM_CTL(port) *(volatile unsigned char*)TM_CTL_ADDR(port)
#define TM_CHN_ADDR(port, chn)\
 (port?  (chn?  0x5a:0x57) : ( chn? (chn-1? (chn-2? 0x1d:0x1a):0x17):0x14 ) )
#define TM_LATCH(port, chn) *(volatile unsigned int*)TM_CHN_ADDR(port, chn)
#define TM_CHNCTL_ADDR(port, chn)\
 (port?  (chn?  0x59:0x56) : ( chn? (chn-1? (chn-2? 0x1c:0x19):0x16):0x13 ) )
#define TM_CHNCTL(port, chn) *(volatile unsigned char*)TM_CHNCTL_ADDR(port,chn)
 
#define TMB_CHN_ADDR(chn) (chn ?  0x5a : 0x57)
#define TMB_LATCH(chn) *(volatile unsigned int*)TMB_CHN_ADDR(chn)
#define TMB_CHNCTL_ADDR(chn) (chn ? 0x59 : 0x56)
#define TMB_CHNCTL(chn) *(volatile unsigned char*)TMB_CHNCTL_ADDR(chn)

/*******************************************************************/
/* PWM REGISTER  MICROS                                            */                                       
/*******************************************************************/
#define PWM_CTL1_ADDR  0x20
#define PWM_CTL1 *(volatile unsigned char*)PWM_CTL1_ADDR
#define PWM_CTL2_ADDR  0x21
#define PWM_CTL2 *(volatile unsigned char*)PWM_CTL2_ADDR
#define PWM_FAULT_CTL_ADDR  0x22
#define PWM_FAULT *(volatile unsigned char*)PWM_FAULT_CTL_ADDR
#define PWM_FAULT_STA_ADDR  0x23
#define PWM_FAULT_STA *(volatile unsigned char*)PWM_FAULT_STA_ADDR
#define PWM_FAULT_ACK_ADDR  0x24
#define PWM_FAULT_ACK *(volatile unsigned char*)PWM_FAULT_ACK_ADDR//
#define PWM_OUT_CTL_ADDR  0x25
#define PWM_OUT_CTL *(volatile unsigned char*)PWM_OUT_CTL_ADDR
#define PWM_DEADTM_ADDR 0x36
#define PWM_DEADTM *(volatile unsigned char*)PWM_DEADTM_ADDR
#define PWM_DISMAP_ADDR 0x37
#define PWM_DISMAP *(volatile unsigned char*)PWM_DISMAP_ADDR
#define PWM_CNT_ADDR  0x26
#define PWM_CNT *(volatile unsigned int*)PWM_CNT_ADDR
#define PWM_MOD_ADDR  0x28
#define PWM_MOD *(volatile unsigned int*)PWM_MOD_ADDR
#define PWM_VALUE_ADDR(chn)\
 ( chn? (chn-1? (chn-2? (chn-3? (chn-4? 0x34:0x32):0x30):0x2e):0x2c ):0x2a )
#define PWM_VALUE(chn) *(volatile unsigned int*)PWM_VALUE_ADDR(chn)

/******************************************************************
*	ANALOG/DIGTAL CONVERTOR MICROS
*******************************************************************/
#define AD_SCR_ADDR 0x40
#define AD_CTL  *(volatile unsigned char *)AD_SCR_ADDR
#define AD_DATAHI_ADDR 0x41
#define AD_DATAHI 	*(volatile unsigned char  *)AD_DATAHI_ADDR
#define AD_DATALO_ADDR 0x42
#define AD_DATALO 	*(volatile unsigned char  *)AD_DATALO_ADDR
#define AD_CLK_ADDR 0x43
#define AD_CLK 	*(volatile unsigned char *)AD_CLK_ADDR
/******************************************************************
* SCI MICROS
*******************************************************************/
#define SCI_SCC1_ADDR 0x38
#define SCI_SCC1  *(volatile unsigned char *)SCI_SCC1_ADDR
#define SCI_SCC2_ADDR 0x39
#define SCI_SCC2  *(volatile unsigned char *)SCI_SCC2_ADDR
#define SCI_SCC3_ADDR 0x3a
#define SCI_SCC3  *(volatile unsigned char *)SCI_SCC3_ADDR
#define SCI_SCS1_ADDR 0x3b
#define SCI_SCS1  *(volatile unsigned char *)SCI_SCS1_ADDR
#define SCI_SCS2_ADDR 0x3c
#define SCI_SCS2  *(volatile unsigned char *)SCI_SCS2_ADDR
#define SCI_SCDR_ADDR 0x3d
#define SCI_SCDR  *(volatile unsigned char *)SCI_SCDR_ADDR
#define SCI_SCBR_ADDR 0x3e
#define SCI_SCBR  *(volatile unsigned char *)SCI_SCBR_ADDR

/******************************************************************
*	BUS FREQUENCY MICRO
*******************************************************************/
#define FQ_XCLK   4000000
#define FQ_VCLK   (1*FQ_XCLK)
#define FQ_OUTCLK (FQ_VCLK/2)
#define FQ_BUSCLK (FQ_OUTCLK/2)
#define FQ_OPCLK  FQ_BUSCLK



#endif  // HC908MRIO_H_INCL
