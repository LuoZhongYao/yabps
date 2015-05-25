#ifndef __CONFIG_PANIC_H__
#define __CONFIG_PANIC_H__

/****************************************************************************
	   
				? Cambridge Silicon Radio Ltd, 2004
 	 			
				All rights reserved

FILE:			config_panic.h  -  wire to an panic-reporting function

CONTAINS:		ABCSP_PANIC  -  report an panic from the abcsp code

DESCRIPTION:	If the abcsp code detects something disastrously wrong, typically
				something that is supposed to be "impossible", it can call
				ABCSP_PANIC().  The single argument reports the apparent disastrous
				occurrence.  This file defines the panic function.

				It is presumed external code will not call any of the abcsp code
				again until it has cleaned up the mess.

REVISION:		$Revision: #1 $ 

*****************************************************************************/

//#include "sched/panic.h" 

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
NAME
        ABCSP_PANIC  -  report an panic from the abcsp code

SYNOPSIS
        void ABCSP_PANIC(abcsp *_this, unsigned e);

FUNCTION
	Reports the occurrence of the panic "e".   Values for "e" are given
	in abcsp_panics.h.

NOTE

*/


//#define ABCSP_PANIC(t, n)	sched_panic(n);
#define ABCSP_PANIC(n)    LOGE(#n)

#ifdef __cplusplus
}
#endif 

#endif  /* __CONFIG_PANIC_H__ */
