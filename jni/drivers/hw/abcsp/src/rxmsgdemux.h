/****************************************************************************
FILE
        rxmsgdemux.h  -  receive message demultiplexor

CONTAINS
        abcsp_rxmsgdemux_init  -  init the receive message demultiplexor
        abcsp_rxmsgdemux_newmsg  -  create a new receive message
        abcsp_rxmsgdemux_putbyte  -  add a byte to a receive message
        abcsp_rxmsgdemux_done  -  signal message is complete and deliver
        abcsp_rxmsgdemux_destroy  -  abandon construction of receive message

DESCRIPTION
	Messages flow up to this code from rxbcsp.[ch].  This uses the
	arguments to abcsp_rxmsgdemux_newmsg() to route BCSP Link
	Establishment messages to le.[ch] code and all other calls to
	rxmsg.[ch] code.

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifndef __RXMSGDEMUX_H__
#define __RXMSGDEMUX_H__

#ifdef RCS_STRINGS
static const char rxmsgdemux_h_id[]
  = "$Id: rxmsgdemux.h,v 1.1 2001/07/06 19:15:39 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_rxmsgdemux_init  -  init the receive message demultiplexor

FUNCTION
        Initialises the receive message demultiplexor.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the demultiplexor.
*/

extern void abcsp_rxmsgdemux_init(void);


/****************************************************************************
NAME
        abcsp_rxmsgdemux_newmsg  -  create a new receive message

FUNCTION
        Destroys any message currently being assembled.

        Creates a fresh message.

        Records that the new message is on BCSP channel "chan".  If "rel"
        is TRUE then this is the reliable channel "chan", else it is
        the unreliable channel "chan".  The new message will be of length
        "len" bytes.

        This sets up message routing for subsequent calls to
        abcsp_rxmsgdemux_putbyte(), abcsp_rxmsgdemux_done() and
        abcsp_rxmsgdemux_destroy().

RETURNS
        TRUE if the fresh message was created, else FALSE.
*/

extern bool abcsp_rxmsgdemux_newmsg(uint16 chan, bool rel, uint16 len);


/****************************************************************************
NAME
        abcsp_rxmsgdemux_putbyte  -  add a byte to a received message

RETURNS
        TRUE if the current message assembler beyond rxmsgdemux has consumed
        the byte "c" and wishes to be given another byte.  FALSE if the byte
        has not been consumed and the assembler does not wish to be called
        again immediately.
*/

extern bool abcsp_rxmsgdemux_putbyte(uint8 c);


/****************************************************************************
NAME
        abcsp_rxmsgdemux_flush  -  push outstanding bytes back to message

FUNCTION
        If the current message assembler behind rxmsgdemux code is
        holding a buffer for the message currently under construction
        this is pushed back into the RXMSG.
*/

extern void abcsp_rxmsgdemux_flush(void);


/****************************************************************************
NAME
        abcsp_rxmsgdemux_done  -  signal message is complete and deliver

FUNCTION
        Marks the message currently being assembled as complete then
        delivers it.
*/

extern void abcsp_rxmsgdemux_done(void);


/****************************************************************************
NAME
        abcsp_rxmsgdemux_destroy  -  abandon construction of received message

FUNCTION
        Destroys any message currently being assembled.
*/

extern void abcsp_rxmsgdemux_destroy(void);


#endif /* __RXMSGDEMUX_H__ */
