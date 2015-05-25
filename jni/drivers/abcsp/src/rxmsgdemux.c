/****************************************************************************
FILE
        rxmsgdemux.c  -  receive message demultiplexor

CONTAINS
        abcsp_rxmsgdemux_init  -  init the receive message demultiplexor
        abcsp_rxmsgdemux_newmsg  -  create a new receive message
        abcsp_rxmsgdemux_putbyte  -  add a byte to a receive message
        abcsp_rxmsgdemux_done  -  signal that receive message is complete
        abcsp_rxmsgdemux_destroy  -  abandon construction of receive message

DESCRIPTION
        This receives received message construction data from rxbcsp.c
        (below) and routes the data (up) to either le.c (link
        establishment messages) or rxmsg.c (all messages for higher
        layers).

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifdef RCS_STRINGS
static const char rxmsgdemux_c_id[]
  = "$Id: rxmsgdemux.c,v 1.1 2001/07/06 19:15:38 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_rxmsgdemux_init  -  init the receive message demultiplexor
*/

void abcsp_rxmsgdemux_init(void)
{
        abcsp_txrx.rxdemux_bcsple_msg = FALSE;
}


/****************************************************************************
NAME
        abcsp_rxmsgdemux_newmsg  -  create a new receive message
*/

bool abcsp_rxmsgdemux_newmsg(uint16 chan, bool rel, uint16 len)
{
        /* Is this a BCSP Link Establishment message? */
        abcsp_txrx.rxdemux_bcsple_msg = (chan == 1 && rel == 0) ? TRUE : FALSE;

        /* Route the new message announcement. */
        if(abcsp_txrx.rxdemux_bcsple_msg)
                return(abcsp_bcsple_newmsg());

        return(abcsp_rxmsg_newmsg(chan, rel, len));
}


/****************************************************************************
NAME
        abcsp_rxmsgdemux_putbyte  -  add a byte to a received message
*/

bool abcsp_rxmsgdemux_putbyte(uint8 c)
{
        /* Route the new message byte. */
        if(abcsp_txrx.rxdemux_bcsple_msg)
                return(abcsp_bcsple_putbyte(c));

        return(abcsp_rxmsg_putbyte(c));
}


/****************************************************************************
NAME
        abcsp_rxmsgdemux_flush  -  push outstanding bytes back to message
*/

void abcsp_rxmsgdemux_flush(void)
{
        /* Route the new message byte. */
        if(abcsp_txrx.rxdemux_bcsple_msg)
                abcsp_bcsple_flush();
        else
                abcsp_rxmsg_flush();
}


/****************************************************************************
NAME
        abcsp_rxmsgdemux_done  -  signal that message is complete
*/

void abcsp_rxmsgdemux_done(void)
{
        /* Route the announcement. */
        if(abcsp_txrx.rxdemux_bcsple_msg)
                abcsp_bcsple_done();
        else
                abcsp_rxmsg_done();
}


/****************************************************************************
NAME
        abcsp_rxmsgdemux_destroy  -  abandon construction of received message
*/

void abcsp_rxmsgdemux_destroy(void)
{
        /* Route the destruction. */
        if(abcsp_txrx.rxdemux_bcsple_msg)
                abcsp_bcsple_destroy();
        else
                abcsp_rxmsg_destroy();
}
