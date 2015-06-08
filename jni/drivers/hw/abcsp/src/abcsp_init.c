/****************************************************************************
FILE
        abcsp_init.c  -  initialise the abcsp library

CONTAINS
        abcsp_init  -  initialise the abcsp library

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifdef RCS_STRINGS
static const char abcsp_init_c_id[]
  = "$Id: abcsp_init.c,v 1.1 2001/07/06 19:15:29 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_init  -  initialise the abcsp library
*/

void abcsp_init( void )
{
        ABCSP_EVENT(ABCSP_EVT_START);

        /* Data shared between the transmit and receive paths. */
        abcsp_txrx_init();

        /* Received message assembler. */
        abcsp_rxmsg_init();

        /* BCSP Link Establishment engine. */
        abcsp_bcsple_init();

        /* Receive message demultiplexor. */
        abcsp_rxmsgdemux_init();

        /* BCSP message decoder. */
        abcsp_rxbcsp_init();

        /* The slip reception code block. */
        abcsp_uart_init();

        /* Transmit message generator. */
        abcsp_txmsg_init();

        /* Transmit message slip encoder. */
        abcsp_txslip_init();

        ABCSP_EVENT(ABCSP_EVT_INITED);
}
