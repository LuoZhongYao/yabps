/****************************************************************************
FILE
        rxmsg.h  -  message assembly

CONTAINS
        abcsp_rxmsg_init  -  initialise the receive message assembler
        abcsp_rxmsg_newmsg  -  create a new receive message
        abcsp_rxmsg_putbyte  -  add a byte to a received message
        abcsp_rxmsg_flush  -  push outstanding bytes back to message
        abcsp_rxmsg_done  -  signal that message is complete and deliver
        abcsp_rxmsg_destroy  -  abandon construction of received message

DESCRIPTION
        Assembles received messages other than BCSP-LE and ack messages,
        i.e., this assembles messages destined to code layered above
        the abcsp library.

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifndef __RXMSG_H__
#define __RXMSG_H__

#ifdef RCS_STRINGS
static const char rxmsg_h_id[]
  = "$Id: rxmsg.h,v 1.1 2001/07/06 19:15:38 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_rxmsg_init  -  initialise the receive message assembler

FUNCTION
        Initialises the receive message assembler, abandoning any work
        in progress.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the assembler.
*/

extern void abcsp_rxmsg_init(void);


/****************************************************************************
NAME
        abcsp_rxmsg_newmsg  -  create a new receive message

FUNCTION
        Destroys any message currently being assembled.

        Creates a fresh message.

        Records that the new message is on BCSP channel "chan".  If "rel"
        is TRUE then this is the reliable channel "chan", else it is the
        unreliable channel "chan".  The message will be "len" bytes long.

RETURNS
        TRUE if the fresh message was created, else FALSE.
*/

extern bool abcsp_rxmsg_newmsg(uint16 chan, bool rel, uint16 len);


/****************************************************************************
NAME
        abcsp_rxmsg_putbyte  -  add a byte to a received message

RETURNS
        TRUE if the assembler has consumed the byte "c" and wishes to
        be given another byte.  FALSE if the byte has not been consumed
        and the assembler does not wish to be called again immediately.
*/

extern bool abcsp_rxmsg_putbyte(uint8 c);


/****************************************************************************
NAME
        abcsp_rxmsg_flush  -  push outstanding bytes back to message

FUNCTION
        If the rxmsg code is holding a buffer from the message currently
        under construction this is passed back into the RXMSG.
*/

extern void abcsp_rxmsg_flush(void);


/****************************************************************************
NAME
        abcsp_rxmsg_done  -  signal that message is complete and deliver

FUNCTION
        Marks the message currently being assembled as complete then
        delivers the message to higher level code.
*/

extern void abcsp_rxmsg_done(void);


/****************************************************************************
NAME
        abcsp_rxmsg_destroy  -  abandon construction of received message

FUNCTION
        Destroys any message currently being assembled.
*/

extern void abcsp_rxmsg_destroy(void);


#endif /* __RXMSG_H__ */
