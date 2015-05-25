/****************************************************************************
FILE
        abcsp_free.c  -  liberate a block of memory

CONTAINS
        abcsp_free  -  liberate a block of memory

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifdef RCS_STRINGS
static const char abcsp_free_c_id[]
  = "$Id: abcsp_free.c,v 1.1 2001/07/06 19:15:28 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_free  -  liberate a block of memory
*/

void abcsp_free(void *p)
{
        ABCSP_FREE(p);
}
