/****************************************************************************
FILE
        txrx.c  -  information used by the transmit and receive paths

CONTAINS
        abcsp_txrx_init  -  initialise the two paths' common information

MODIFICATION HISTORY
        1.1   6:jul:00  cjo     Created.
*/

#ifdef RCS_STRINGS
static const char txrx_c_id[]
  = "$Id: txrx.c,v 1.1 2001/07/06 19:15:42 cjo Exp $";
#endif

#include <string.h>
#include "abcsp_private.h"

/* Database of info common to tx and rx paths. */
TXRXINFO abcsp_txrx;


/****************************************************************************
NAME
        abcsp_txrx_init  -  initialise the two paths' common information
*/

void abcsp_txrx_init(void)
{
        (void) memset((void*)(&abcsp_txrx), 0x00, sizeof(TXRXINFO));
        abcsp_txrx.choke = TRUE;
        abcsp_txrx.bcsple_muzzled = TRUE;
}
