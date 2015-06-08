/****************************************************************************
FILE
        txmsg.h  -  message transmission

CONTAINS
        abcsp_txmsg_init  -  initialise the transmit message assembler
        abcsp_sendmsg  -  set up message for sending to the uart
        abcsp_pumptxmsgs  -  send message to the uart

DESCRIPTION
        This combines the transmit elements of the Sequencing, Datagram
        Queue, Mux and Packet Integrity Layers from the BCSP specification,
        bc01-s-006e (aka AN004).

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifndef __TXMSG_H__
#define __TXMSG_H__

#ifdef RCS_STRINGS
static const char txmsg_h_id[]
  = "$Id: txmsg.h,v 1.1 2001/07/06 19:15:42 cjo Exp $";
#endif

#include "abcsp_private.h"


/* The BCSP transmit packet crc.   Needs to be global as it is initialised
and used by txmsg.c, but calculated per-character in txslip.c. */

extern uint16 abcsp_txmsg_crc;


/****************************************************************************
NAME
        abcsp_txmsg_init  -  initialise the transmit message assembler

FUNCTION
        Initialises the transmit message assembler, abandoning any work
        in progress.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the assembler.
*/

extern void abcsp_txmsg_init(void);


/****************************************************************************
NAME
        abcsp_sendmsg  -  set up message for sending to the uart
*/

/* Defined in abcsp.h. */


/****************************************************************************
NAME
        abcsp_pumptxmsgs  -  send message to the uart
*/

/* Defined in abcsp.h. */


#endif /* __TXMSG_H__ */
