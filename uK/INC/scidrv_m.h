/*==============================================================================

FILE NAME:  
    scidrv_m.h

PURPOSE:
    sci driver

FUNCTION(S):
    SCIDRV_mINIT - initialize sci
    SCIDRV_mSET_BAUD - set baud rate
	SCIDRV_mTX_EN - enable sci transmit
	SCIDRV_mRX_EN - enable sci receive   
	SCIDRV_mTXINTR_EN - enable transmit interrupt
	SCIDRV_mCTL - sci control
    SCIDRV_mSTA_READ - read sci status
    SCI_DATA_READ - read sci data
    SCIDRV_mWRITE - write sci data
    SCIDRV_mTX_COMPLETE - query transmit complete
    SCIDRV_mRX_COMPLETE - query receive complete
	SCIDRV_mERROR - query sci error    
	SCIDRV_mCLOSE - close sci
    
Author: John Qin
Date: 6/23/2003  

    
==============================================================================*/
#ifndef SCIDRV_M_H_INCL
#define SCIDRV_M_H_INCL
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
#define SCI_FRAMEERR (0x02)
#define SCI_NOISE	 (0x04)
#define SCI_OVERFLOW (0x08)
#define SCI_IDLE	 (0x10)
#define SCI_SCRF	 (0x20)
#define	SCI_TC		 (0x40)
#define	SCI_SCTE	 (0x80)
#define SCI_TX       (0x03)
#define SCI_RX       (0x02)
#define SCI_TX_INTR  (0x07)
#define SCI_OFF      (0x00)
#define SCBDH_BTST   (0x80)  /* Baud register test */
#define SCBDH_BSPL   (0x40)  /* Baud rate counter split (test) */
#define SCBDH_SBRH   (0x3f)  /* Mask for high bits of SCBR selects 
						   */
#define SCBDH_SBRL   (0xff)  /* Mask for low bits of SCBR selects*/
#define SCCR1_LOOP   (0x80)  /* SCI Loop mode enable (active high) */
#define SCCR1_WOMS   (0x40)  /* Wired-OR mode for SCI pins (active high) */
#define SCCR1_M      (0x10)  /* Mode select - 0 = 8x1, 1 = 9x1. SICK. */
#define SCCR1_WAKE   (0x80)  /* Wakeup by address mark/idle: 0 = IDLE, 1 = MARK */
#define SCCR1_ILT    (0x40)  /* Idle line type 0 = short, 1 = long.  Tells when
						    * to start counting idle line time - wait for
						    * stop bit or not 
						    */
#define SCCR1_PE     (0x20)  /* Parity enable: 0 = off, 1 = on */
#define SCCR1_PT     (0x10)  /* Parity type: 0 = even, 1 = odd */
#define SCCR2_TIE    (0x80)  /* Transmit interrupt enable: 0 = off, 1 = on */
#define SCCR2_TCIE   (0x40)  /* Transmit complete interrupt enable */
#define SCCR2_RIE    (0x20)  /* SCI Receiver interrupt enable when RDRF flag or 
						    * the OR status flag is set
						    */
#define SCCR2_ILIE   (0x01)  /* Idle line interrupt enable */
#define SCCR2_TE     (0x08)  /* Transmitter enable */
#define SCCR2_RE     (0x04)  /* Receiver enable */
#define SCCR2_RWU    (0x02)  /* Receiver wakeup control: 
						    * 0 = normal SCI receiver
						    * 1 = rx interrupts disabled
						    */
#define SCCR2_SBK    (0x01)  /* Send Break enable */
#define SCSR1_SCTE   (0x80)  /* Transmit data register empty flag */
#define SCSR1_TC     (0x40)  /* Transmit complete flag (0 = busy) */
#define SCSR1_RDRF   (0x20)  /* Receive data register full flag (1 = full) */
#define SCSR1_IDLE   (0x10)  /* Idle line detected flag (1 = rx idle) */
#define SCSR1_OR     (0x08)  /* Over-run error flag (1 = overrun) */
#define SCSR1_NF     (0x04)  /* Noise error flag (1 = noise detected) */
#define SCSR1_FE     (0x02)  /* Framing error flag (1 = framing error) */
#define SCSR1_PF     (0x01)  /* Parity error flag (1 = parity error) */
#define SCSR2_RAF    (0x01)  /* Receiver active flag (RO) */

#define RX_CHAR_TIMEOUT (30000)
#define SCI_DATAERROR_MASK (SCI_FRAMEERR+SCI_NOISE)



/*
*Default: 8-bit, 1-stop, non-loop, idle line-wake, short, 
*no parity mode, 9600 baud rate, SCI enabled
*enable receive interrupts RX 
*/
#define SCI_DF_SCC1  (0x44)// 0x44/0x40
#define SCI_DF_SCC2  (0x24)//0x34/0x24
#define SCI_DF_SCC3  (0x00)//0x0f/0x00
#define SCI_24H_SCBR (0x11) //clk src:1MHz, 3:2
#define SCI_48H_SCBR (0x21) //4:2
#define SCI_96H_SCBR (0x10) //(0x10)7.37Mz(1.8432Mz)
//#define SCI_96H_SCBR (0x01) //(0x01)4.9152Mz;(0x10)7.37Mz(1.8432Mz)

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
 *SCI initialization, 9600 baud rate selected
 *8-bit, non-loop, idle line-wake, short, 
 *no parity mode, 9600 baud rate, SCI enabled
 *enable transmit complete and receive interrupts, TX/RX 
*/ 
#define SCIDRV_mINIT()\
 { REG_WRITE(&SCI_SCC1, SCI_DF_SCC1);\
   REG_WRITE(&SCI_SCC2, SCI_DF_SCC2);\
   REG_WRITE(&SCI_SCC3, SCI_DF_SCC3);\
   REG_WRITE(&SCI_SCBR, SCI_96H_SCBR);}
#define SCIDRV_mSET_BAUD(baud)\
   REG_WRITE(&SCI_SCBR, baud)  
#define SCIDRV_mTX_EN(toggle)\
    (toggle? SET_BIT(&SCI_SCC2, SCI_TX):RESET_BIT(&SCI_SCC2, SCI_TX))
#define SCIDRV_mRX_EN(toggle)\
    (toggle? SET_BIT(&SCI_SCC2, SCI_RX):RESET_BIT(&SCI_SCC2, SCI_RX))
#define SCIDRV_mTXINTR_EN(toggle)\
    (toggle? SET_BIT(&SCI_SCC2, SCI_TX_INTR):RESET_BIT(&SCI_SCC2, SCI_TX_INTR))
#define SCIDRV_mCTL(ctrl1, ctrl2, ctrl3)\
 { REG_WRITE(&SCI_SCC1, ctrl1);\
   REG_WRITE(&SCI_SCC2, ctrl2);\
   REG_WRITE(&SCI_SCC3, ctrl3);}        
#define SCIDRV_mSTA_READ()\
    ( SCI_SCS1 | (SCI_SCS2 << 8) )
#define SCI_DATA_READ()\
    ( SCI_SCDR )   
#define SCIDRV_mWRITE(byte)\
    { REG_WRITE(&SCI_SCDR, byte);}           

#define SCIDRV_mTX_COMPLETE() ( PORT_READ ( SCSR1_BASE ) & SCSR1_TC )
#define SCIDRV_mRX_COMPLETE() ( PORT_READ ( SCSR1_BASE ) & SCSR1_RDRF )
#define SCIDRV_mERROR()   ( PORT_READ ( SCSR1_BASE ) & ( SCSR1_OR | \
						     SCSR1_NF | \
						     SCSR1_FE | \
						     SCSR1_PF ) )
#define SCIDRV_mCLOSE()\
  {  REG_WRITE(&SCI_SCC1, SCI_OFF);\
     REG_WRITE(&SCI_SCC2, SCI_OFF);\
     REG_WRITE(&SCI_SCC3, SCI_OFF);\
     REG_WRITE(&SCI_SCBR, SCI_OFF);}
     
#endif //SCIDRV_M_H_INCL     
   
