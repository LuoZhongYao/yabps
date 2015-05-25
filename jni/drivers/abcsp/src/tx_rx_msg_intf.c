/****************************************************************************

                (c) Cambridge Silicon Radio Ltd, 2006

                All rights reserved

FILE:           tx_rx_msg_intf.c  -  _abcsp _tx and _rx message interface support

DESCRIPTION:    abcsp tx msg interface functions

                abcsp_rxmsg_create  -
                abcsp_rxmsg_getbuf  -
                abcsp_rxmsg_write  -
                abcsp_rxmsg_complete  -
                temp_abcsp_rxmsg_destroy  -
                abcsp_txmsg_init_read  -
                abcsp_txmsg_length  -
                abcsp_txmsg_getbuf  -
                abcsp_txmsg_taken  -
                abcsp_txmsg_done  -

REVISION:       $Revision: #2 $

***************************************************************************/

#include "abcsp.h"
#include "config_event.h"
#include "config_panic.h"
#include "config_rxmsg.h"
#include "config_timer.h"
#include "config_txmsg.h"
#include "config_le.h"
//#include "bchs_tasks.h"
//#include "hc_com.h"
#include "abcsp_private.h"
//#include "queue_lib.h"


/****************************************************************************
 *NAME
 *        ABCSP_RXMSG *abcsp_rxmsg_create(unsigned len)
 *
 *
 *FUNCTION
 *        Create a struct from malloc.
 *        bufLen is the size of the data length
 *        dex is used for index
 *
 *        Create a Buffer for the Received data.
 *
 *RETURNS
 *        The pointe of the structure generated
 */

void *abcsp_rxmsg_create(unsigned int len)
{

#ifdef ABCSP_OPTIMIZED
    if (len > ABCSP_RXMSG_MAX_PAYLOAD_LEN)
    {
        ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
        return (NULL);
    }
    rxMsgStruct.buflen = len;
    rxMsgStruct.dex = 0;
    rxMsgStruct.buf = NULL;

    return((&rxMsgStruct));

#else /* ABCSP_OPTIMIZED */

#ifndef FIXED_ABCSP_RX_BUFFER

    MessageStructure *ret = (MessageStructure*) malloc(sizeof(MessageStructure));

    ret->buflen = len;
    ret->buf = (char*) malloc(len);
    ret->dex = 0;

    return((ABCSP_RXMSG*)(ret));
#else /* FIXED_ABCSP_RX_BUFFER*/
    if (len > ABCSP_RXMSG_MAX_PAYLOAD_LEN)
    {
        ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
        return (NULL);
    }
    rxMsgStruct.buflen = len;
    rxMsgStruct.dex = 0;
    rxMsgStruct.buf = (char*) bufData;

    return((&rxMsgStruct));

#endif /* FIXED_ABCSP_RX_BUFFER */
#endif /* ABCSP_OPTIMIZED */
}

/****************************************************************************
 *NAME
 *        char *abcsp_rxmsg_getbuf(ABCSP_RXMSG *m, unsigned *n)
 *FUNCTION
 *        Calculate the free space in buffer
 *RETURNS
 *        The free space in buffer
 *        Return the pointer of the free space onwards
 */
#ifndef ABCSP_OPTIMIZED
char *abcsp_rxmsg_getbuf(ABCSP_RXMSG *m, unsigned *n)
{
#ifndef FIXED_ABCSP_RX_BUFFER

    MessageStructure *mm = (MessageStructure*)(m);
    *n = mm->buflen - mm->dex;

    return(&(mm->buf[mm->dex]));
#else /*FIXED_ABCSP_RX_BUFFER */

    *n = rxMsgStruct.buflen - rxMsgStruct.dex;
    return (&(rxMsgStruct.buf[rxMsgStruct.dex]));
#endif /* FIXED_ABCSP_RX_BUFFER */

}
#endif /* ABCSP_OPTIMIZED */

/****************************************************************************
 *NAME
 *        void abcsp_rxmsg_write(ABCSP_RXMSG *m, char *buf, unsigned n)
 *FUNCTION
 *        Update the index pointer.
 *RETURNS
 *        NON
 */
#ifndef ABCSP_OPTIMIZED
void abcsp_rxmsg_write(ABCSP_RXMSG *m, char *buf, unsigned n)
{
#ifndef FIXED_ABCSP_RX_BUFFER

    MessageStructure *mm = (MessageStructure*)(m);

    mm->dex +=n;
#else /*FIXED_ABCSP_RX_BUFFER */

    rxMsgStruct.dex += n;
#endif /* FIXED_ABCSP_RX_BUFFER */
}
#endif /* ABCSP_OPTIMIZED */


/****************************************************************************
 *NAME
 *        void abcsp_rxmsg_complete(ABCSP_RXMSG *m)
 *FUNCTION
 *        Tells that the message is complete and thus that no more
 *        calls will be made to abcsp_rxmsg_getbuf or abcsp_rxmsg_write
 *RETURNS
 *        NON
 */
void abcsp_rxmsg_complete(ABCSP_RXMSG *m)
{

}


/****************************************************************************
 *NAME
 *        void temp_abcsp_rxmsg_destroy(ABCSP_RXMSG *m)
 *FUNCTION
 *        Destroy the Rx message buffer and structure
 *RETURNS
 *        NON
 */


void AbcspRxMmsgDestroy(ABCSP_RXMSG *m)
{
#ifndef ABCSP_OPTIMIZED
#ifndef FIXED_ABCSP_RX_BUFFER

    MessageStructure *mm = (MessageStructure*)(m);

    free((void*)(mm->buf));
    free((void*)(mm));

#endif /*FIXED_ABCSP_RX_BUFFER*/
#endif /* ABCSP_OPTIMIZED */

}

/****************************************************************************
 *NAME
 *        void abcsp_txmsg_init_read(ABCSP_TXMSG *msg)
 *FUNCTION
 *        Set the index pointer to start
 *RETURNS
 *        NON
 */

void abcsp_txmsg_init_read(void *msg)
{

    MessageStructure *mm = (MessageStructure*)(msg);
    mm->dex = 0;
}


/****************************************************************************
 *NAME
 *        unsigned abcsp_txmsg_length(ABCSP_TXMSG *msg)
 *FUNCTION
 *
 *RETURNS
 *        how many payload bytes are in a message.
 */
unsigned abcsp_txmsg_length(ABCSP_TXMSG *msg)
{

    MessageStructure *mm = (MessageStructure*)(msg);

    return(mm->buflen);
}


/****************************************************************************
 *NAME
 *        char *abcsp_txmsg_getbuf(ABCSP_TXMSG *msg, unsigned *buflen)
 *FUNCTION
 *
 *RETURNS
 *        The remaining buffer space
 *        A pointer to the free space, if no more bytes return NULL
 */
char *abcsp_txmsg_getbuf(ABCSP_TXMSG *msg, unsigned *buflen)
{
    MessageStructure *mm = (MessageStructure*)(msg);
    *buflen = mm->buflen - mm->dex;

    if (*buflen == 0)
    {
        return (NULL);
    }
    else
    {
        return(&(mm->buf[mm->dex]));
    }
}


/****************************************************************************
 *NAME
 *        void abcsp_txmsg_taken(ABCSP_TXMSG *msg, unsigned ntaken)
 *FUNCTION
 *        Update the number o bytes been consumed from the buffer obtained
 *        by the preceding call to abcsp_txmsg_getbuf.
 *
 *RETURNS
 *        NON
 */
void abcsp_txmsg_taken(ABCSP_TXMSG *msg, unsigned ntaken)
{

    MessageStructure *mm = (MessageStructure*)(msg);
    mm->dex += ntaken;
}


/****************************************************************************
 *NAME
 *        void abcsp_txmsg_done(ABCSP_TXMSG *msg)
 *FUNCTION
 *        For an unreliable BCSP message (SCO) this means that the message has been send.
 *        For a reliable BCSP message this means that the message has been send to the peer
 *        and that the peer has returned acknowledgement of its reception. This can
 *        be used as the basics of a "recorded delivery" mechanism.
 *
 *RETURNS
 *        NON
 */
void abcsp_txmsg_done(ABCSP_TXMSG *msg)
{
    MessageStructure *mm = (MessageStructure*)(msg);

    free((void*)(mm->buf));
    free((void*)(mm));
}

