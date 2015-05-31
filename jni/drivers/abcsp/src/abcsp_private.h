/****************************************************************************
FILE
        abcsp_private.h  -  internal abcsp common header

CONTAINS
        MALLOC  -  allocate a block of memory
        NEW  -  allocate a typed block of memory
        ZMALLOC  -  allocate a block of zeroed memory
        ZNEW  -  allocate a block of typed zeroed memory
        FREE  -  liberate a block of memory
        DELETE  -  carelessly liberate a block of memory
        abcsp_malloc  -  allocate a block of memory
        abcsp_free  -  liberate a block of memory
        abcsp_zmalloc  -  allocate a block of memory

DESCRIPTION
        Header file to be #included by all of the abcsp code in the abcsp
        library.

        This #includes the extra #include files needed by files in this
        directory.  It also includes any descriptions and declarations
        needed locally by these files.

	This file declares functions that wire to the environment's heap/pool
	memory allocation functions plus some supporting macros.  The abcsp
	library code uses the heap/pool macros (named in capital letters)
	rather than the (lower case) functions.

        This file must not be #included by files outside this directory.

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Ripped off from the bc01 code's bt.h, then
                                extensively refurbished.
*/

#ifndef __ABCSP_PRIVATE_H__
#define __ABCSP_PRIVATE_H__

#ifdef RCS_STRINGS
static const char abcsp_private_h_id[]
  = "$Id: abcsp_private.h,v 1.1 2001/07/06 19:15:31 cjo Exp $";
#endif

#include "commonc.h"
#include "chw.h"
#include "abcsp.h"
#include "txrx.h"
#include "config_event.h"
#include "config_panic.h"
#include "config_malloc.h"
#include "config_timer.h"
#include "rxslip.h"
#include "rxbcsp.h"
#include "rxmsgdemux.h"
#include "rxmsg.h"
#include "txmsg.h"
#include "txslip.h"
#include "le.h"
#include "crc.h"
#include "abcsp_panics.h"
#include "abcsp_events.h"

#ifdef ABCSP_TEST
#include "abcsp_test.h"
#endif /* ABCSP_TEST */ 


/****************************************************************************
NAME
        MALLOC  -  allocate a block of memory

SYNOPSIS
        void *MALLOC(uint16 n)

FUNCTION
        As the standard C library function, malloc().

RETURNS
        A block of memory of size "n" bytes, or NULL if it could not be
        obtained.
*/

#define MALLOC(n)       abcsp_malloc(n)


/****************************************************************************
NAME
        NEW  -  allocate a typed block of memory

SYNOPSIS
        void *NEW(t)

FUNCTION
	Macro to allocate a block of memory of type "t" and cast the memory's
	base address to be of type "*t".

RETURNS
        A block of memory of large enough to contain a data element of type
        "t", or NULL if it could not be obtained.
*/

#undef NEW
#define NEW(t)          ((t*)(MALLOC(sizeof(t)))


/****************************************************************************
NAME
        ZMALLOC  -  allocate a block of zeroed memory

SYNOPSIS
        void *ZMALLOC(uint16 n)

FUNCTION
        As the standard C library function, calloc(1,n).

RETURNS
        A zeroed block of memory of size "n" bytes, or NULL if it could not
        be obtained.
*/

#define ZMALLOC(n)      abcsp_zmalloc(n)


/****************************************************************************
NAME
        ZNEW  -  allocate a block of typed zeroed memory

SYNOPSIS
        void *ZNEW(t)

FUNCTION
        Macro to allocate a block of memory of type "t", clear all of its
        locations to zero then cast the memory's base address to be of
        type "*t".

RETURNS
        A zeroed block of memory of large enough to contain a data element of
        type "t", or NULL if it could not be obtained.
*/

#define ZNEW(t)         ((t*)(ZMALLOC(sizeof(t))))


/****************************************************************************
NAME
        FREE  -  liberate a block of memory

SYNOPSIS
        void FREE(void *p);

FUNCTION
        As the standard C library function, free().

        The block of memory "p", previously obtained via one of the macros
        and functions defined in this file, is returned to its source.
*/

#define FREE(p)         abcsp_free(p)


/****************************************************************************
NAME
        DELETE  -  carelessly liberate a block of memory

SYNOPSIS
        void DELETE(void *p);

FUNCTION
        As the standard C library function, free().

        The block of memory "p", previously obtained via one of the macros
        defined in this file, is returned to its source.

        This macro wrapping of FREE() allows the caller to be careless
        of the type of the memory returned.
*/
#undef DELETE
#define DELETE(p)       (FREE((void*)(p)))


/****************************************************************************
NAME
        abcsp_malloc  -  allocate a block of memory

FUNCTION
        As the standard C library function, malloc().

RETURNS
        A block of memory of size "n" bytes, or NULL if it could not be
        obtained.
*/

extern void *abcsp_malloc(uint16 n);


/****************************************************************************
NAME
        abcsp_zmalloc  -  allocate a block of memory

FUNCTION
        As the standard C library function, calloc(1,n).

RETURNS
        A zeroed block of memory of size "n" bytes, or NULL if it could not
        be obtained.
*/

extern void *abcsp_zmalloc(uint16 n);


/****************************************************************************
NAME
        abcsp_free  -  liberate a block of memory

FUNCTION
        As the standard C library function, free().

        The block of memory "p", previously obtained via abcsp_malloc()
        or abcsp_zmalloc() defined in this file, is returned to its source.
*/

extern void abcsp_free(void *p);


#endif  /* __ABCSP_PRIVATE_H__ */
