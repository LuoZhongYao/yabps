/****************************************************************************
FILE
        config_malloc.h  -  wire to the environment's heap/pool functions

CONTAINS
        ABCSP_MALLOC  -  allocate a block of memory
        ABCSP_ZMALLOC  -  allocate a block of zeroed memory
        ABCSP_FREE  -  liberate a block of memory

DESCRIPTION
        The environment is required to provide functions that behave in ways
        similar to the standard C library functions malloc(), calloc() and
        free().  This file should be configured to wire to these functions.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __CONFIG_MALLOC_H__
#define __CONFIG_MALLOC_H__
 
#ifdef RCS_STRINGS
static const char config_malloc_h_id[]
  = "$Id: config_malloc.h,v 1.1 2001/07/06 19:14:10 cjo Exp $";
#endif

#ifdef ABCSP_TEST
#include <stdlib.h>
#endif /* ABCSP_TEST */


/****************************************************************************
NAME
        ABCSP_MALLOC  -  allocate a block of memory

SYNOPSIS
        void *ABCSP_MALLOC(unsigned n)

FUNCTION
        Required to work in a manner similar to the standard C library
        function malloc().
*/

#ifdef ABCSP_TEST
#undef  ABCSP_MALLOC
#define ABCSP_MALLOC(n)          malloc(n)
#endif /* ABCSP_TEST */


/****************************************************************************
NAME
        ABCSP_ZMALLOC  -  allocate a block of zeroed memory

SYNOPSIS
        void *ABCSP_ZMALLOC(unsigned n)

FUNCTION
        Required to work in a manner similar to the standard C library
        function calloc(1,n).
*/

#ifdef ABCSP_TEST
#undef  ABCSP_ZMALLOC
#define ABCSP_ZMALLOC(n)         calloc(1,n)
#endif /* ABCSP_TEST */


/****************************************************************************
NAME
        ABCSP_FREE  -  liberate a block of memory

SYNOPSIS
        void ABCSP_FREE(void *p);

FUNCTION
        Required to work in a manner similar to the standard C library
        function free().
*/

#ifdef ABCSP_TEST
#undef  ABCSP_FREE
#define ABCSP_FREE(n)            free(n)
#endif /* ABCSP_TEST */


#endif  /* __CONFIG_MALLOC_H__ */
