/****************************************************************************
  FILE
  rxmsg.c  -  receive a bcsp packet

  CONTAINS
  abcsp_rxmsg_init  -  initialise the receive message assembler
  abcsp_rxmsg_newmsg  -  create a new receive message
  abcsp_rxmsg_putbyte  -  add a byte to a received message
  abcsp_rxmsg_flush  -  push outstanding bytes back to message
  abcsp_rxmsg_done  -  signal that message is complete
  abcsp_rxmsg_destroy  -  abandon construction of received message

  clear_rxmsg  -  clear rxmsg storage

  MODIFICATION HISTORY
  1.1   6:jul:01   cjo    Created.
  */

#ifdef RCS_STRINGS
static const char rxmsg_c_id[]
= "$Id: rxmsg.c,v 1.1 2001/07/06 19:15:37 cjo Exp $";
#endif

#include "abcsp_private.h"
#include "config_rxmsg.h"


/* The message currently being constructed. */
static ABCSP_RXMSG *rxmsg;

/* Byte buffer for rxmsg. */
static char *rxmsg_buf;
static unsigned rxmsg_index;
static unsigned rxmsg_buflen;

/* Message channel info. */
static uint8 rxmsg_chan;
static bool rxmsg_rel;

/* Forward reference. */
static void clear_rxmsg(void);


/****************************************************************************
  NAME
  abcsp_rxmsg_init  -  initialise the receive message assembler
  */

void abcsp_rxmsg_init(void)
{
    abcsp_rxmsg_destroy();
}


/****************************************************************************
  NAME
  abcsp_rxmsg_newmsg  -  create a new receive message
  */

bool abcsp_rxmsg_newmsg(uint16 chan, bool rel, uint16 len)
{
    abcsp_rxmsg_destroy();
    rxmsg_chan = (uint8)(chan);
    rxmsg_rel = rel;
    return(((rxmsg = ABCSP_RXMSG_CREATE((unsigned)(len)))
                != (ABCSP_RXMSG*)(NULL))
            ? TRUE : FALSE);
}


/****************************************************************************
  NAME
  abcsp_rxmsg_putbyte  -  add a byte to a received message
  */

bool abcsp_rxmsg_putbyte(uint8 c)
{
    if(rxmsg == (ABCSP_RXMSG*)(NULL))
        return(FALSE);
    if(rxmsg_buf == (char*)(NULL)) {
        if((rxmsg_buf = ABCSP_RXMSG_GETBUF(rxmsg, &rxmsg_buflen))
                == (char*)(NULL))
            return(FALSE);
        if(rxmsg_buflen == 0) {
            ABCSP_RXMSG_WRITE(rxmsg, rxmsg_buf, 0);
            return(FALSE);
        }
        rxmsg_index = 0;
    }

    if(rxmsg_index < rxmsg_buflen)
        rxmsg_buf[rxmsg_index++] = (char)(c);

    if(rxmsg_index >= rxmsg_buflen)
        abcsp_rxmsg_flush();

    return(TRUE);
}


/****************************************************************************
  NAME
  abcsp_rxmsg_flush  -  push outstanding bytes back to message
  */

void abcsp_rxmsg_flush(void)
{
    if(rxmsg == (ABCSP_RXMSG*)(NULL))
        return;
    if(rxmsg_buf != (char*)(NULL)) {
        ABCSP_RXMSG_WRITE(rxmsg, rxmsg_buf, rxmsg_index);
        rxmsg_buf = (char*)(NULL);
    }
}


/****************************************************************************
  NAME
  abcsp_rxmsg_done  -  signal that message is complete and deliver
  */

void abcsp_rxmsg_done(void)
{
    if(rxmsg == (ABCSP_RXMSG*)(NULL))
        return;
    abcsp_rxmsg_flush();
    ABCSP_RXMSG_COMPLETE(rxmsg);
    ABCSP_DELIVERMSG(rxmsg, (unsigned)(rxmsg_chan),
            (unsigned)(rxmsg_rel));
    clear_rxmsg();
}


/****************************************************************************
  NAME
  abcsp_rxmsg_destroy  -  abandon construction of received message
  */

void abcsp_rxmsg_destroy(void)
{
    if(rxmsg != (ABCSP_RXMSG*)(NULL))
        ABCSP_RXMSG_DESTROY(rxmsg);
    clear_rxmsg();
}


/****************************************************************************
  NAME
  clear_rxmsg  -  clear rxmsg storage

  FUNCTION
  Clears all local storage used to hold a receive message.
  */

    static
void clear_rxmsg(void)
{
    rxmsg = (ABCSP_RXMSG*)(NULL);
    rxmsg_buf = (char*)(NULL);
    rxmsg_index = 0;
    rxmsg_buflen = 0;
}
