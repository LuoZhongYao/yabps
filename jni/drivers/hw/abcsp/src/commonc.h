/****************************************************************************
FILE
        commonc.h  -  system-wide common header file

DESCRIPTION
        This should be #included by every C source file in the library.

        The file includes typedefs and #defines that are common to many
        C projects.

        This file should be changed only infrequently and with great care.

	This file should contain the bare minimum necessary to get the job
	done; it should not become a dumping ground for quick-fix globals.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Copied from bc01 code.
*/

#ifndef __COMMONC_H__
#define __COMMONC_H__

#include <stdbool.h>
 
#ifdef RCS_STRINGS
static const char commonc_h_id[]
  = "$Id: commonc.h,v 1.1 2001/07/06 19:15:32 cjo Exp $";
#endif


#undef  NULL
#define NULL            (0)

#if 0
#undef  bool
#define bool            unsigned
#endif

#undef  fast
#define fast            register

#undef  TRUE
#define TRUE            (1)

#undef  FALSE
#define FALSE           (0)

#undef  forever
#define forever         for(;;)

#undef  max
#define max(a,b)        (((a) > (b)) ? (a) : (b))

#undef  min
#define min(a,b)        (((a) < (b)) ? (a) : (b))

/* To shut lint up. */
#undef  unused
#define unused(x)       (void)x


#endif  /* __COMMONC_H__ */
