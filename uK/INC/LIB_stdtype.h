/*==============================================================================

FILE NAME:  
    LIB_stdtype.h

PURPOSE:
    Architecture independant data definitions

FUNCTION(S):
    None
   
    local:
        None
   
Author: John Qin
Date: 6/23/2003  

==============================================================================*/

#ifndef LIB_STDTYPE_INCL
#define LIB_STDTYPE_INCL


/*==============================================================================
                              Includes
==============================================================================*/
//Define types based on processor architecture
#if defined( _8BIT_ARCHITECTURE )
#include "LIB_stdtype8.h"

#elif defined( _16BIT_ARCHITECTURE )
#include "LIB_stdtype16.h"

#elif defined(  _32BIT_ARCHITECTURE )
#include "LIB_stdtype32.h"

#else
#error ARCHITECTURE not defined
#endif


#endif // LIB_STDTYPE_INCL
