/*==============================================================================

FILE NAME:  
    uKUtil.h
PURPOSE:
	OS utilities
FUNCTION(S):
    Util_fnGet_bit - get a bit in a byte
    Util_fnSet_bit - set a bit in a byte
    Util_fnClear_bit - clear a bit in a byte
    Util_fnWrite_byte - write bits in a byte
    Util_fnRBuf_PutByte - insert a byte to a ring buffer
    Util_fnRBuf_GetByte - get a byte from a ring buffer
    Util_fnRBuf_Export - export data from ring buffer to other buffer
    Util_fnRBuf_Search - search a data in a ring buffer
    Util_fnChar_To_Hex - conver char to hex
    Util_fnCrcCal - calculate CRC
	
Author: John Qin
Date: 6/23/2003  

==============================================================================*/
#ifndef UKUTIL_H_INCL
#define UKUTIL_H_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    // Requires and architecture defn. in the compiler
#include "LIB_stddefs.h"    // Standard definitions for all projects


/*==============================================================================
                              Defines
==============================================================================*/
#ifndef RB_BUF_SIZE
#define RB_BUF_SIZE1	( 10 )
#define RB_BUF_SIZE2	( 2 )//6
#endif

#define QUEUE_SIZE      ( 4 )
#define CRCREG			( 0xFF )
/*==============================================================================
                            Type Definitions
==============================================================================*/

typedef struct zRingbuf
{
	uchar8 ucHead;
	uchar8 ucTail;
	uchar8 aucData[RB_BUF_SIZE2];
} uKERNEL_tzRingBuf;


typedef struct zQUEUE
{ uint8   size;
  uint8   head;
  uint8   tail;
  uint8   avail;
  uchar8  buf[QUEUE_SIZE];
} uKERNEL_tzQUEUE;



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


/*==============================================================================
                      External/Public Function Protoypes
==============================================================================*/
#define CONTENTS_OF(addr) \
        (*((volatile unsigned char*)(addr)))

#define LENGTH_TO_BITS(length) \
        (~(0xffffffff << (length)))

#define REG_READ(addr) \
        (CONTENTS_OF(addr))

#define REG_WRITE(addr,val) \
        (CONTENTS_OF(addr) = (val))

#define MASK_BIT(bit) \
        (1 << (bit))

#define RESET_BIT(addr,bit) \
        (CONTENTS_OF(addr) &= (~MASK_BIT(bit)))

#define GET_BIT(addr,bit) \
        (CONTENTS_OF(addr) & (MASK_BIT(bit)) ? 1 : 0)

#define SET_BIT(addr,bit) \
        (CONTENTS_OF(addr) = (CONTENTS_OF(addr)) | (MASK_BIT(bit)))

#define ASSIGN_BIT_VAL(addr,bit,val) \
        ( (val) ? SET_BIT(addr,bit) : RESET_BIT(addr,bit) )

#define CREATE_FIELD(bit,length) \
        (LENGTH_TO_BITS(length) << (bit))

#define RESET_FIELD(addr,bit,length) \
        ( CONTENTS_OF(addr) &= (~CREATE_FIELD(bit,length)))

#define TRUNCATE(val,bit,length) \
        (((unsigned int)(val) << (bit)) & (CREATE_FIELD(bit, length)))

#define MASK_FIELD(bit,val,length)\
        TRUNCATE(val, bit, length)

#define GET_FIELD(addr,bit,length) \
       ((CONTENTS_OF(addr) & CREATE_FIELD(bit,length)) >> bit)

#define LOAD_FIELD(addr,val,bit,length) \
        (CONTENTS_OF(addr) &= (~CREATE_FIELD(bit,length))\
                               | TRUNCATE(val, bit, length))  

#define LO_BYTE(x) 		  ((unsigned char)((x) & 0xff))
#define HI_BYTE(x) 		  ((unsigned char)((unsigned short)(x) >> 8))
#define mLO_BYTE(x) 		  ((unsigned char)((x) & 0xff))
#define mHI_BYTE(x) 		  ((unsigned char)((unsigned short)(x) >> 8))

#define PORT_WRITE(a,b)\
		*((volatile unsigned char *)a) = b		
#define PORT_READ(a)\
	    *((volatile unsigned char *)a)

#define uKERNEL_mRB_PUTBYTE(rb, ucByte)\
    { rb.aucData[rb.ucTail++] = ucByte;}
#define uKERNEL_mRB_GETBYTE(rb)  (rb.aucData[rb.ucHead++])
 //((RBUF_EMPTY()?(rb.aData[rb.ucHead++]):0))
#define uKERNEL_mRBUF_INIT(rb)\
 	{rb.ucHead = 0; rb.ucTail = 0;}  
#define uKERNEL_mRBUF_EMPTY(rb) (rb.ucHead == rb.ucTail)
#define uKERNEL_mRBUF_FULL(rb,size)  (rb.ucTail >= size)
#define uKERNEL_mCLEAR_RBUF(rb) (rb.ucTail = 0;rb.ucHead = 0;)
#define uKERNEL_mRESET_RBUF(rb)\
 	{rb.ucHead = 0; rb.ucTail = 0;}  

	    		 
#define RBuf_Init(rb)\
 	//{rb.ucCount = 0;\
 	{rb.ucHead = 0;\
 	 rb.ucTail = 0;}
#define RBuf_IsFull(rb,size) (rb.ucTail >= size)
#define RBuf_IsEmpty(rb) (rb.ucHead == rb.ucTail)
#define RBuf_Count(rb) (rb.ucCount)
#define queue_empty(queue) (queue)->head == (queue)->tail
#define queue_avail(queue) (queue)->avail



#define CRC_CAL(crc, dataByte)\
	  { crc = (CRCREG^dataByte);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1);\
   	    crc = (crc & 0x80)?((crc<<1)^0x8C):(crc<<1); }
	  
uchar8 Util_fnGet_bit(uchar8 bpos, volatile uchar8 *addr);
void Util_fnSet_bit(uchar8 bpos, volatile uchar8  * addr);
void Util_fnClear_bit(uchar8 bpos, volatile uchar8  * addr);
void Util_fnWrite_byte(uchar8 value, volatile uchar8  * addr);
uchar8 Util_fnRBuf_PutByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 ucByte);
schar8 Util_fnRBuf_GetByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize);
void Util_fnRBuf_Export(uKERNEL_tzRingBuf *pBuf,  uchar8 bufSize, uchar8 *buf, uchar8 *len);
uint8 Util_fnRBuf_Search(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 msgId);
uchar8 Util_fnCrcCal(uchar8 crcOld, uchar8 dataByte);

#endif //UKUTIL_H_INCL
