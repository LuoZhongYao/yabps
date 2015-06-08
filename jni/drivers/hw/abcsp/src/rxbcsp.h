/****************************************************************************
FILE
        rxbcsp.h  -  message assembly

CONTAINS
        abcsp_rxbcsp_init  -  initialise the message assembler
        abcsp_rxbcsp_putbyte  -  add a byte to a received message
        abcsp_rxbcsp_done  -  signal that message is complete
        abcsp_rxbcsp_destroy  -  abandon construction of received message

DESCRIPTION
        Form a message from a received de-slipped byte stream.  This
        implements the Packet Integrity, Mux, Sequencing and Datagram
        Queue BCSP stack entities for the receive stream.

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifndef __RXBCSP_H__
#define __RXBCSP_H__

#ifdef RCS_STRINGS
static const char rxbcsp_h_id[]
  = "$Id: rxbcsp.h,v 1.1 2001/07/06 19:15:36 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_rxbcsp_init  -  initialise the message assembler

FUNCTION
        Initialises the message assembler, abandoning work in progress.

        This must be called before all other functions declared in this
        file.
*/

extern void abcsp_rxbcsp_init(void);


/****************************************************************************
NAME
        abcsp_rxbcsp_putbyte  -  add a byte to a received message

FUNCTION
        Pushes the byte "c" into the received message assembler.

RETURNS
        TRUE if the assembler has consumed the byte "c" and wishes to
        be given another byte.  FALSE if the byte has not been consumed
        and the assembler does not wish to be called again immediately.
*/

extern bool abcsp_rxbcsp_putbyte(uint8 c);


/****************************************************************************
NAME
        abcsp_rxbcsp_done  -  signal that message is complete

FUNCTION
        Tells the received message assembler that the message is complete.
*/

extern void abcsp_rxbcsp_done(void);


/****************************************************************************
NAME
        abcsp_rxbcsp_destroy  -  abandon construction of received message

FUNCTION
        Tells the received message assembler to abandon the construction
        of any current message.
*/

extern void abcsp_rxbcsp_destroy(void);


#endif /* __RXBCSP_H__ */
