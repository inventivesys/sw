/*==============================================================================

FILE NAME:  
    uKutil

PURPOSE:
    utility functions

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
   
    local:

   
Author: John Qin
Date: 6/23/2003  
    
==============================================================================*/


/*==============================================================================
                              Includes
==============================================================================*/
#include "HC908MRIO.h"
#include "uKutil.h"

/*==============================================================================
                              Defines
==============================================================================*/


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


/*==============================================================================
                        Local/Private Function Protoypes
==============================================================================*/


/*==============================================================================
                           Function Definitions
==============================================================================*/

/*==============================================================================
                        Bit Operation Functions
==============================================================================*/

	
/*******************************************************************************

FUNCTION NAME: 
    uchar8 Util_fnGet_bit(uchar8 bpos, volatile uchar8 * addr)
  
PURPOSE: 
    get a bit from a byte at addr. 

INPUTS: 
    bpos:bit position; 
    addr: address at which operation taken

OUTPUTS:
    a bit

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uchar8 Util_fnGet_bit(uchar8 bpos, volatile uchar8 * addr)
{
     	uchar8 m;
     	
     	m = *((uchar8 *) addr);
		return( (m &= 1 << (bpos-1)) != 0 );
}

/*******************************************************************************

FUNCTION NAME: 
    void Util_fnSet_bit(uchar8 bpos, volatile uchar8  * addr)
  
PURPOSE: 
     set a bit in a byte at addr. 

INPUTS: 
    bpos:bit position; 
    addr: address at which operation taken

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void Util_fnSet_bit(uchar8 bpos, volatile uchar8  * addr)
{
		uchar8 *m;
		
     	m = ((uchar8 *) addr);
		if ( bpos > 8 )
		{
     		*m |= 0xff;
			addr = m;			
		}
		else
		{
	     	*m |= 1 << bpos;
			addr = m;
		}
}
/*******************************************************************************

FUNCTION NAME: 
    void Util_fnClear_bit(uchar8 bpos, volatile uchar8  * addr)
  
PURPOSE: 
      clear a bit in a byte at addr. 

INPUTS: 
    bpos:bit position; 
    addr: address at which operation taken

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void Util_fnClear_bit(uchar8 bpos, volatile uchar8  * addr)
{
		uchar8 *m;
		
     	m = ((uchar8 *) addr);
		if ( bpos > 8 )
		{
     		*m &= 0x00;
			addr = m;			
		}
		else
		{
			*m &= ~(1 << bpos);
			addr = m;			
		}
}
/*******************************************************************************

FUNCTION NAME: 
    void Util_fnWrite_byte(uchar8 value, volatile uchar8  * addr)
  
PURPOSE: 
     write bits in a byte at addr. 

INPUTS: 
    value:bit value 
    addr: address at which operation taken

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
void Util_fnWrite_byte(uchar8 value, volatile uchar8  * addr)
{
		uchar8 *m;
		
     	m = ((uchar8 *) addr);
		{
			*m |= value;
			addr = m;			
		}
}



/*******************************************************************************

FUNCTION NAME: 
    uchar8 Util_fnRBuf_PutByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 ucByte)
  
PURPOSE: 
    inserts a byte into the ring buffer 

INPUTS: 
    pBuf: pointer to ring buffer 
    ucByte: byte to insert

OUTPUTS:
    status

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uchar8 Util_fnRBuf_PutByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 ucByte)
{
    uint8 res = 1;
	
	pBuf->aucData[pBuf->ucTail++] = ucByte;
	
	if( (pBuf->ucTail >= bufSize) )
	{
	    pBuf->ucTail = 0;
	    res = 0;//buffer is rest
	}

	return res;
}

/*******************************************************************************

FUNCTION NAME: 
    schar8 Util_fnRBuf_GetByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize)

  
PURPOSE: 
     retrieves a byte from the ring buffer 

INPUTS: 
    pBuf: pointer to ring buffer 
    bufSize: buffer size

OUTPUTS:
    status

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
schar8 Util_fnRBuf_GetByte(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize)
{
	uchar8 uReturn;

	if ( (pBuf->ucHead == pBuf->ucTail) )
	{
	    uReturn = -1;//buffer has no un-readed data
	} else if( pBuf->ucHead == bufSize )
	{
		pBuf->ucHead = 0;
	} else
	{
		uReturn = pBuf->aucData[pBuf->ucHead++];
	}
	return uReturn;
}

/*******************************************************************************

FUNCTION NAME: 
    uchar8 Util_fnRBuf_Export(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 *buf, uchar8 *len)

  
PURPOSE: 
     export ring buffer data to other buffer 

INPUTS: 
    pBuf: pointer to ring buffer
    bufSize: buffer size
    buf: pointer to other buffer
    len: size been exported

OUTPUTS:
    none

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/

void Util_fnRBuf_Export(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 *buf, uchar8 *len)
{
    buf = &pBuf->aucData[0];
    *len = bufSize;
}
/*******************************************************************************

FUNCTION NAME: 
    uint8 Util_fnRBuf_Search(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 msgId)

  
PURPOSE: 
     search ring buffer for the first matched id, then return a word 

INPUTS: 
    pBuf: pointer to ring buffer 
    bufSize: buffer size
    msgId: data id

OUTPUTS:
    a word

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uint8 Util_fnRBuf_Search(uKERNEL_tzRingBuf *pBuf, uchar8 bufSize, uchar8 msgId)
{
	uchar8 i;
	
	for (i=0; i<bufSize-1; i++)
	{
		/*the highest 4 bits is msg id, then the following byte is data*/
		if ( (pBuf->aucData[i] >> 4) == msgId )
		{
			return ( (pBuf->aucData[i] << 8) | pBuf->aucData[i+1] );
		}
	}
	return 0;
}
/*******************************************************************************

FUNCTION NAME: 
     schar8 Util_fnChar_To_Hex(char c)
  
PURPOSE: 
     conver a char to hex value 

INPUTS: 
    c: character to convert 
   

OUTPUTS:
    ascii 

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
schar8 Util_fnChar_To_Hex(schar8 c)	
{
	if (c < 10) 
	{
		c = c + 0x30;
	} 
	else 
	{
		c = c + 0x37;
	}
	return c;
}
/*******************************************************************************

FUNCTION NAME: 
     uchar8 Util_fnCrcCal(uchar8 crcOld, uchar8 dataByte)
  
PURPOSE: 
     calculate CRC 

INPUTS: 
    crcOld: old CRC value 
    dataByte: data to be caculated

OUTPUTS:
    CRC

NOTES:
    <notes>

VERSION HISTORY:

Version: 1.00  Date: Oct. 10/5/03 By John Qin
    - Created.
    

*******************************************************************************/
uchar8 Util_fnCrcCal(uchar8 crcOld, uchar8 dataByte)
{
	uchar8 crc;
	uchar8 count;
	
	//crc = (0xFF^dataByte);
	crc = (crcOld^dataByte);
	for ( count=0; count<8; count++)
	{
		if ( (crc & 0x80) )
		{
			crc = ((crc<<1)^0x8C);
		} 
		else 
		{
			crc = (crc<<1);
		}
	}

	return crc;
}

