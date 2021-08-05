/*==============================================================================

FILE NAME:  
    LIB_stddefs.h

PURPOSE:
    This is a header file containing all the data definitions to be used 
    as common entities in all Xantrex projects.

FUNCTION(S):
    None
   
    local:
        None
   
Author: John Qin
Date: 6/23/2003  

==============================================================================*/
 
#ifndef LIB_STDDEFS_INCL
#define LIB_STDDEFS_INCL

/*==============================================================================
                              Includes
==============================================================================*/
#include "LIB_stdtype.h"    //Requres and architecture defintion in the compiler


/*==============================================================================
                              Defines
==============================================================================*/

#if defined(__HIWARE__)         // If compiler is for HCS12
#if defined(__BIG_ENDIAN__)     // If compiler uses integers in Big Endian order
#define XT_BIG_ENDIAN           // Define Xantrex Big Endian
#endif
#endif

#ifndef NULL
#define NULL                   ((void *) 0)
#endif

#ifndef FALSE
#define  FALSE  0
#endif

#ifndef TRUE
#define  TRUE   ( !FALSE )
#endif

#define  OFF    0
#define  ON     ( !OFF )

#define  NO     0
#define  YES    ( !NO )

//BIT positions
#define BITPOS0  0
#define BITPOS1  1
#define BITPOS2  2
#define BITPOS3  3
#define BITPOS4  4
#define BITPOS5  5
#define BITPOS6  6
#define BITPOS7  7
#define BITPOS8  8
#define BITPOS9  9
#define BITPOS10 10
#define BITPOS11 11
#define BITPOS12 12
#define BITPOS13 13
#define BITPOS14 14
#define BITPOS15 15
#define BITPOS16 16
#define BITPOS17 17
#define BITPOS18 18
#define BITPOS19 19
#define BITPOS20 20
#define BITPOS21 21
#define BITPOS22 22
#define BITPOS23 23
#define BITPOS24 24
#define BITPOS25 25
#define BITPOS26 26
#define BITPOS27 27
#define BITPOS28 28
#define BITPOS29 29
#define BITPOS30 30
#define BITPOS31 31


#if defined( _8BIT_ARCHITECTURE )
#define BIT0   0x01
#define BIT1   0x02
#define BIT2   0x04
#define BIT3   0x08
#define BIT4   0x10
#define BIT5   0x20
#define BIT6   0x40
#define BIT7   0x80

#elif defined( _16BIT_ARCHITECTURE )
#define BIT0   0x0001
#define BIT1   0x0002
#define BIT2   0x0004
#define BIT3   0x0008
#define BIT4   0x0010
#define BIT5   0x0020
#define BIT6   0x0040
#define BIT7   0x0080
#define BIT8   0x0100
#define BIT9   0x0200
#define BIT10  0x0400
#define BIT11  0x0800
#define BIT12  0x1000
#define BIT13  0x2000
#define BIT14  0x4000
#define BIT15  0x8000

#elif defined( _24BIT_ARCHITECTURE )
#define BIT0   0x000001
#define BIT1   0x000002
#define BIT2   0x000004
#define BIT3   0x000008
#define BIT4   0x000010
#define BIT5   0x000020
#define BIT6   0x000040
#define BIT7   0x000080
#define BIT8   0x000100
#define BIT9   0x000200
#define BIT10  0x000400
#define BIT11  0x000800
#define BIT12  0x001000
#define BIT13  0x002000
#define BIT14  0x004000
#define BIT15  0x008000
#define BIT16  0x010000
#define BIT17  0x020000
#define BIT18  0x040000
#define BIT19  0x080000
#define BIT20  0x100000
#define BIT21  0x200000
#define BIT22  0x400000
#define BIT23  0x800000

#elif defined( _32BIT_ARCHITECTURE )
#define BIT0   0x00000001
#define BIT1   0x00000002
#define BIT2   0x00000004
#define BIT3   0x00000008
#define BIT4   0x00000010
#define BIT5   0x00000020
#define BIT6   0x00000040
#define BIT7   0x00000080
#define BIT8   0x00000100
#define BIT9   0x00000200
#define BIT10  0x00000400
#define BIT11  0x00000800
#define BIT12  0x00001000
#define BIT13  0x00002000
#define BIT14  0x00004000
#define BIT15  0x00008000
#define BIT16  0x00010000
#define BIT17  0x00020000
#define BIT18  0x00040000
#define BIT19  0x00080000
#define BIT20  0x00100000
#define BIT21  0x00200000
#define BIT22  0x00400000
#define BIT23  0x00800000
#define BIT24  0x01000000
#define BIT25  0x02000000
#define BIT26  0x04000000
#define BIT27  0x08000000
#define BIT28  0x10000000
#define BIT29  0x20000000
#define BIT30  0x40000000
#define BIT31  0x80000000

#else
#error ARCHITECTURE not defined
#endif  //ifdef _8BIT_ARCHITECTURE

/*==============================================================================
                            Type Definitions
==============================================================================*/
//"Status" codes for returns and/or general purpose use.  This idea is that in
//general, a fn can return a value as per eSTATUS.  If the value returned is
//eSTATUS_OK, all is good.  If the value returned is eSTATUS_ERR, then the
//receiver can look at an error variable that details the error.

#if defined( _8BIT_ARCHITECTURE )
typedef uint8  tuiSTATUS;

#elif defined( _16BIT_ARCHITECTURE )
typedef uint16 tuiSTATUS;

#elif defined( _32BIT_ARCHITECTURE )
typedef uint32 tuiSTATUS;

#else
#error ARCHITECTURE not defined

#endif  //ifdef _8BIT_ARCHITECTURE


/*==============================================================================
                                Enums
==============================================================================*/
//The types of status that can are available (tuiSTATUS).
typedef enum eSTATUS
{
    eSTATUS_INVALID = 0,    //Not to be used
    eSTATUS_OK,             //No issues
    eSTATUS_ERR             //Issue occured or exists
     
}teSTATUS;


/*=============================================================================
                              Macros
==============================================================================*/
//The following macro implements the common way to use tuiSTATUS and set an
//errno code.  tuiStatus needs to be declared in order for this macro to work.
#define mWRITE_STATUS_AND_ERRNO( ErrNoCode )    tuiStatus = eSTATUS_ERR;\
                                                errno     = ErrNoCode

/*=============================================================================
                              Structures
==============================================================================*/



#endif  //LIB_STDDEFS_INCL
