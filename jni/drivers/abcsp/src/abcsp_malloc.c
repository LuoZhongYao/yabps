/****************************************************************************
FILE
        abcsp_malloc.c  -  allocate a block of memory

CONTAINS
        abcsp_malloc  -  allocate a block of memory

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifdef RCS_STRINGS
static const char abcsp_malloc_c_id[]
  = "$Id: abcsp_malloc.c,v 1.1 2001/07/06 19:15:30 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_malloc  -  allocate a block of memory
*/

void *abcsp_malloc(uint16 n)
{
        void *ret = ABCSP_MALLOC(n);

        /* Moan if the pool has run dry. */
        if(ret == (void*)(NULL))
                ABCSP_EVENT(ABCSP_EVT_MALLOC);

        return(ret);
}
