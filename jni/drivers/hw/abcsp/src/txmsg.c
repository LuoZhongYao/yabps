/****************************************************************************
FILE
        txmsg.c  -  transmit a bcsp packet

CONTAINS
        abcsp_txmsg_init  -  initialise the transmit message assembler
        abcsp_sendmsg  -  set up message for sending to the uart
        abcsp_pumptxmsgs  -  send message to the uart
        abcsp_bcsp_timed_event  -  transmit path's timed event

        pkt_avail  -  is there a new transmit message available
        pkt_cull  -  discard transmit messages no longer needed
        get_txmsgbuf  -  get a block of transmit message bytes
        txmsgbuf_taken  -  how many source bytes have been taken
        restart_timeout  -  stop and restart the bcsp timer
        slip_one_byte  -  slip one byte and send it to the uart
        msgq_pop_destroy  -  destroy a message from the base of a queue
        msgq_add  -  add a message to the end of a queue
        msgq_pop  -  take a message from the base of a queue
        msgq_match  -  match an entry in a message queue
        msgq_len  -  how many messages are in a queue
        incrmod8  -  increment, modulo 8
        decrmod8  -  decrement, modulo 8

IMPLEMENTATION NOTES
        The core of this code is a pair of queues, implemented as linked
        lists, used to hold messages submitted via abcsp_sendmsg().

          The first queue, relq, is used for reliable messages - the code
          limits the queue's length to the BCSP transmit window size.

          The second list holds only a single message for the unreliable
          message path.  Whenever a message is submitted to this queue
          it displaces (discards) any existing message in the queue.
          This mechanism is to support sco (voice) traffic, which is at
          its most valueable when delivered fresh.

        There are two other sources of messages:

          The BCSP Link Establishment entity, which runs on the receive path
          of the abcsp stack, can request this code to emit four types of
          four byte messages.

          The abcsp stack's receive path can request that this code emit
          acknowledgements of packets received.  An ack value is normally
          piggybacked onto any packet sent by this code, but if no piggy
          is available this code sends an ack packet.

        A state machine selects the highest priority message from the four
        available sources (the priority order is fixed) then builds a BCSP
        message around it (BCSP header, payload and optional CRC), then
        passes the message to the txslip.c code for translation into a SLIP
        frame.

        The main processing mechanism is for higher-level, external code to
        post to one of the two queues via abcsp_sendmsg() then to call
        abcsp_pumptxmsgs() until the message is sent.  The other two message
        sources are enabled by the abcsp library's receive path setting flags
        in abcsp_txrx, but the external code is still required to call
        abcsp_pumptxmsgs() to generate and transmit the BCSP messages.

        When any element of the abcsp library requires external code to call
        abcsp_pumptxmsgs() it calls ABCSP_REQ_PUMPTXMSGS().  For example, the
        BCSP Link Establishment code raises this event if it wishes the
        txmsg.c code to emit a link establishment message.

        This file uses the single timer controlled by ABCSP_START_BCSP_TIMER()
        and ABCSP_CANCEL_BCSP_TIMER().  If this (external code) timer fires
        the external code is required to call abcsp_bcsp_timed_event().

MODIFICATION HISTORY
        1.1   06:jul:01  cjo    Created.
        1.2   28:sep:01  cjo    Comment corrected.
        1.3   23:apr:02  cjo    Call abcsp_crc_reverse() only once per
				transmitted slip packet.  Issue ID AB.4.
	1.4   19:feb:03  mm01   AB.6: Miscalculation of header checksum.
*/

#ifdef RCS_STRINGS
static const char txmsg_c_id[]
  = "$Id: txmsg.c,v 1.4 2003/02/19 20:19:47 mm01 Exp $";
#endif

#include "abcsp_private.h"
#include "config_txmsg.h"

#define TAG "ABCSP"

/* A message submitted to abcsp_sendmsg(). */
typedef struct txmsg {
        struct txmsg    *next;  /* Next in linked list. */
        ABCSP_TXMSG     *m;     /* The message (reference) itself. */
        unsigned        chan:4; /* BCSP channel. */
        unsigned        seq:3;  /* Sequence number. */
        } TXMSG;    

/* An internally-generated message (bcsp-le or ack). */
typedef struct {
        uint8           *buf;   /* Message byte buffer. */
        unsigned        outdex:4; /* Buffer extraction point. */
        unsigned        chan:4; /* BCSP channel. */
        } INTMSG;

/* The message currently being transmitted. */
typedef struct {
        uint16          len;    /* Message length. */
        bool            rel:1;  /* Reliable message? */
/* Discriminated union. */
#define CURRMSG_TYPE_TXMSG      (0)
#define CURRMSG_TYPE_INTMSG     (1)
        unsigned        type:1; /* Discrim: TXMSG or internally-generated. */
        union {
                TXMSG   *txmsg;
                INTMSG  inter;
                } m;
        } CURRMSG;
static CURRMSG curr;

/* Queues for reliable and unreliable abcsp_sendmsg() messages. */
static TXMSG *relq;
static TXMSG *unrelq;

/* Message builder's state. */
enum txstate_enum {
        txstate_idle,           /* Waiting for new message to send. */
        txstate_msgstart,       /* Sending slip frame start byte. */
        txstate_hdr1,           /* Sending first bcsp header byte. */
        txstate_hdr2,           /* Sending second bcsp header byte. */
        txstate_hdr3,           /* Sending third bcsp header byte. */
        txstate_hdr4,           /* Sending last bcsp header byte. */
        txstate_payload,        /* Sending message payload. */
        txstate_crc1,           /* Sending first crc byte. */
        txstate_crc2,           /* Sending first crc byte. */
        txstate_msgend          /* Sending slip frame end byte. */
        };
typedef enum txstate_enum txstate;
static txstate state;

/* Reliable packet sequence number - used to assign seq to each rel pkt. */
static uint8 msgq_txseq;

/* Reliable packet sequence number - used by fsm to emit packets. */
static uint8 txseq;

/* BCSP packet header checksum. */
static uint8 cs;

/* BCSP packet crc. */
uint16 abcsp_txmsg_crc;


/* Forward references. */
static unsigned xabcsp_pumptxmsgs(void);
static bool pkt_avail(void);
static void pkt_cull(void);
static uint8 *get_txmsgbuf(uint16 *len);
static void restart_timeout(void);
static void txmsgbuf_taken(uint16 ntaken);
static bool slip_one_byte(uint8 b);
static void msgq_add(TXMSG **q, TXMSG *m);
static bool msgq_pop_destroy(TXMSG **q);
static TXMSG *msgq_pop(TXMSG **q);
static TXMSG *msgq_match(TXMSG *q, uint8 seq);
static uint16 msgq_len(TXMSG *q);
static uint8 incrmod8(uint8 n);
static uint8 decrmod8(uint8 n);


/****************************************************************************
NAME
        abcsp_txmsg_init  -  initialise the transmit message assembler
*/

void abcsp_txmsg_init(void)
{
        ABCSP_CANCEL_BCSP_TIMER();
        state = txstate_idle;
        msgq_txseq = 0;
        txseq = 0;

        /* Clear the two message queues. */
        while(msgq_pop_destroy(&relq))
                ;
        while(msgq_pop_destroy(&unrelq))
                ;
}


/****************************************************************************
NAME
        abcsp_sendmsg  -  set up message for sending to the uart
*/

unsigned abcsp_sendmsg(ABCSP_TXMSG *msg, unsigned chan, unsigned rel)
{
        TXMSG *m;

        /* Reject all traffic if the choke is applied.

        BCSP-LE messages are transmitted from code below this entry point.

        The choke should be applied at the "mux" layer.  Applying it here
        means that if the choke is turned on while messages are queued for
        transmission then those messages will drain out.  This is strictly
        incorrect, but this won't harm any real system as the choke is only
        set TRUE by abcsp library init, so any peer is going to see
        disrupted traffic for a while anyway.  (Ideally, bcsp-le messages
        from here will tell the peer that we've restarted, so it should
        reinit and rechoke.) */

        if(abcsp_txrx.choke) {
                ABCSP_EVENT(ABCSP_EVT_TX_CHOKE_DISCARD);
                return(0);
                }

        /* Parameter sanity checks. */
        if(rel > 1 || chan < 2 || chan > 15 || msg == (ABCSP_TXMSG*)(NULL))
                return(0);

        /* We queue enough reliable messages to fill the WINSIZE window. */
        if(rel && msgq_len(relq) >= ABCSP_TXWINSIZE) {
                ABCSP_EVENT(ABCSP_EVT_TX_WINDOW_FULL_DISCARD);
                return(0);
                }

        /* Package the message. */
        if((m = ZNEW(TXMSG)) == (TXMSG*)(NULL))
                return(0);
        m->m = msg;
        m->chan = chan;

        if(rel) {
                /* We've already checked the reliable queue has room. */
                m->seq = msgq_txseq;
                msgq_txseq = incrmod8(msgq_txseq);
                msgq_add(&relq, m);
                }
        else {
                /* The unreliable channel is biased towards supporting
                sco, for which the data has to be fresh.  The queue
                holds only one message, so we displace any message that's
                already in the queue. */

                (void) msgq_pop_destroy(&unrelq);

                msgq_add(&unrelq, m);
                }

        /* Tell external code that it needs to call abcsp_pumptxmsgs(). */
        ABCSP_REQ_PUMPTXMSGS();

        /* Report message accepted. */
        return(1);
}


/****************************************************************************
NAME
        abcsp_pumptxmsgs  -  send message to the uart
*/

unsigned abcsp_pumptxmsgs(void)
{
        /* Prepare bytes for passing to the UART. */
        unsigned ret = xabcsp_pumptxmsgs();

        /* Push (once!) the prepared bytes to the UART. */
        abcsp_txslip_flush();

        /* Report whether any work remains to be done immediately. */
        return(ret);
}


/****************************************************************************
NAME
        xabcsp_pumptxmsgs  -  send message to the uart

FUNCTION
        As abcsp_pumptxmsgs(), except that this does not send messages out
        to the UART via ABCSP_UART_SENDBYTES().  This function obtains a
        buffer via ABCSP_UART_GETBUF() and may put slipped bytes into this,
        but it does not actually push the buffer through to the UART.
*/

static
unsigned xabcsp_pumptxmsgs(void)
{
        uint8 *buf, b;
        uint16 buflen;
        bool more;

        forever {
                switch(state) {
                    default:
                    case txstate_idle:
                        /* Discard any acknowledged reliable messages. */
                        pkt_cull();

                        /* Choose pkt to send.  Writes directly to "curr". */
                        if(! pkt_avail())
                                return(0);
                        state = txstate_msgstart;

                        /* Sanity check. */
                        if(curr.len > ABCSP_MAX_MSG_LEN)
                                ABCSP_PANIC(ABCSP_PANIC_BCSP_MSG_OVERSIZE);

                        break;

                    case txstate_msgstart:
                        if(abcsp_txslip_msgstart() == FALSE)
                                return(1);
#ifdef  ABCSP_TXCRC
                        abcsp_crc_init(&abcsp_txmsg_crc);
                        abcsp_txrx.txcrc = TRUE;
#endif  /* ABCSP_TXCRC */
                        state = txstate_hdr1;
                        break;

                    case txstate_hdr1:
                        /* The txack value is 1 more than the seq of the
                        received packet being acknowledged.  However, the
                        rxseq_txack value was incremented by the receive
                        path, so we use the value of rxseq_txack directly. */

                        b = abcsp_txrx.rxseq_txack << 3;
                        abcsp_txrx.txack_req = FALSE;
                        if(curr.rel)
                                b |= 0x80 + curr.m.txmsg->seq;
#ifdef  ABCSP_TXCRC
                        b |= 0x40;
#endif  /* ABCSP_TXCRC */
                        cs = b;
                        if(!slip_one_byte(b))
                                return(1);
                        state = txstate_hdr2;
                        break;

                    case txstate_hdr2:
                        b = curr.len << 4;
                        if(curr.type == CURRMSG_TYPE_INTMSG)
                                b |= curr.m.inter.chan;
                        else
                                b |= curr.m.txmsg->chan;
                        if(!slip_one_byte(b))
                                return(1);
                        cs += b;
                        state = txstate_hdr3;
                        break;

                    case txstate_hdr3:
                        b = curr.len >> 4;
                        if(!slip_one_byte(b))
                                return(1);
                        cs += b;
                        state = txstate_hdr4;
                        break;

                    case txstate_hdr4:
                        if(!slip_one_byte(~cs))
                                return(1);
                        state = txstate_payload;
                        break;

                    case txstate_payload:
                        if((buf = get_txmsgbuf(&buflen)) != (uint8*)(NULL)) {
                                txmsgbuf_taken(abcsp_txslip_sendbuf(buf,
                                                buflen, &more));
                                if(!more)
                                        return(1);
                                }
                        else {
                                /* Destroy the message if it's unreliable and
                                externally generated (main example: sco).
                                We send this sort of message only once, and
                                it's already been removed from the unrelq in
                                pkt_avail(). */

                                if(curr.type == CURRMSG_TYPE_TXMSG &&
                                                curr.rel == FALSE) {
                                        ABCSP_TXMSG_DONE(curr.m.txmsg->m);
                                        DELETE(curr.m.txmsg); 
                                        }

#ifndef ABCSP_TXCRC
                                state = txstate_msgend;
                                }
                        break;

#else  /* ABCSP_TXCRC */
                        	abcsp_txrx.txcrc = FALSE;
                        	abcsp_txmsg_crc 
					  = abcsp_crc_reverse(abcsp_txmsg_crc);
                                state = txstate_crc1;
                                }
                        break;

                    case txstate_crc1:
                        if(!slip_one_byte((uint8)((abcsp_txmsg_crc >> 8)
                                        & 0x00ff)))
                                return(1);
                        state = txstate_crc2;
                        break;

                    case txstate_crc2:
                        if(!slip_one_byte((uint8)(abcsp_txmsg_crc & 0x00ff)))
                                return(1);
                        state = txstate_msgend;
                        break;

#endif  /* ABCSP_TXCRC */

                    case txstate_msgend:
                        if(abcsp_txslip_msgend() == FALSE)
                                return(1);
                        state = txstate_idle;
                        break;
                    }
                }

        /* LINTED Can't get here - this is just to shut lint up. */
        return(0);
}


/****************************************************************************
NAME
        abcsp_bcsp_timed_event  -  transmit path's timed event
*/

void abcsp_bcsp_timed_event(void)
{
        uint8 new_txseq;

        /* Arrange to retransmit all messages in the relq. */
        if(relq) {
                if((new_txseq = relq->seq) != txseq) {
                        txseq = new_txseq;
                        ABCSP_REQ_PUMPTXMSGS();
                        restart_timeout();
                        }
                }
}


/****************************************************************************
NAME
        pkt_avail  -  is there a new transmit message available

FUNCTION
        This determines which of the available transmit messages should
        be sent next to the UART, i.e., it implements the message
        prioritisation.

        The function also configures this file's state ready for the
        chosen message's transmission.

RETURNS
        TRUE if a message is available to be sent, else FALSE.

        If the function returns TRUE then details of the packet to be sent
        have been written directly to this file's static variable "curr".
*/

static
bool pkt_avail(void)
{
        TXMSG *tmpm;

        /* Default settings - an ack packet. */
        curr.rel = FALSE;
        curr.len = 0;
        curr.type = CURRMSG_TYPE_INTMSG;
        curr.m.inter.outdex = 0;
        curr.m.inter.chan = 0;
        curr.m.inter.buf = (uint8*)(NULL);

        /* BCSP-LE messages have the highest priority.  (Always unrel.) */
        if(abcsp_txrx.txsync_req || abcsp_txrx.txsyncresp_req ||
                        abcsp_txrx.txconf_req || abcsp_txrx.txconfresp_req) {
                curr.len = 4;
                curr.m.inter.chan = 1;
                if(abcsp_txrx.txsync_req) {
                        abcsp_txrx.txsync_req = FALSE;
                        curr.m.inter.buf = (uint8*)(abcsp_le_sync_msg);
                        }
                else if(abcsp_txrx.txsyncresp_req) {
                        abcsp_txrx.txsyncresp_req = FALSE;
                        curr.m.inter.buf = (uint8*)(abcsp_le_syncresp_msg);
                        }
                else if(abcsp_txrx.txconf_req) {
                        abcsp_txrx.txconf_req = FALSE;
                        curr.m.inter.buf = (uint8*)(abcsp_le_conf_msg);
                        }
                else if(abcsp_txrx.txconfresp_req) {
                        abcsp_txrx.txconfresp_req = FALSE;
                        curr.m.inter.buf = (uint8*)(abcsp_le_confresp_msg);
                        }
                return(TRUE);
                }

        /* Any unreliable TXMSG has the next highest priority. */
        if(msgq_len(unrelq) > 0) {
                /* We *remove* the message from the queue.   We will destroy
                it after sending it once.  (This is different from the way in
                which relq is used, where messages remain in the queue until
                their reception is acknowledged by the peer.) */

                curr.type = CURRMSG_TYPE_TXMSG;
                curr.m.txmsg = msgq_pop(&unrelq);
                ABCSP_TXMSG_INIT_READ(curr.m.txmsg->m);
                curr.len = ABCSP_TXMSG_LENGTH(curr.m.txmsg->m);
                return(TRUE);
                }

        /* Any reliable data is next in the pecking order. */
        if((tmpm = msgq_match(relq, txseq)) != (TXMSG*)(NULL)) {
                curr.type = CURRMSG_TYPE_TXMSG;
                curr.m.txmsg = tmpm;
                txseq = incrmod8(txseq);
                ABCSP_TXMSG_INIT_READ(curr.m.txmsg->m);
                curr.len = ABCSP_TXMSG_LENGTH(curr.m.txmsg->m);
                curr.rel = TRUE;
                restart_timeout();
                return(TRUE);
                }

        /* Finally, send an ack packet if needed. */
        if(abcsp_txrx.txack_req) {
                /* All messages send ack val, so clear the flag later.  This
                also fights race hazard if tx and rx threads are separate. */

                return(TRUE);
                }

        /* No message available. */
        return(FALSE);
}


/****************************************************************************
NAME
        pkt_cull  -  discard transmit messages no longer needed

FUNCTION
        Determines which reliable packets have been received (acked)
        by the peer and discards the local copy of these if they are in
        the reliable transmit queue.
*/

static
void pkt_cull(void)
{
        uint8 rxack, seq;
        TXMSG *m;

        /* Give up immediately if there's nothing to cull. */
        if(! msgq_len(relq))
                return;

        /* The received ack value is always one more than the seq of
        the packet being acknowledged. */

        rxack = decrmod8(abcsp_txrx.rxack);

        /* abcsp_txrx.rxack carries the ack value from the last packet
        received from the peer.  It marks the high water mark of packets
        accepted by the peer.  We only use it if it refers to a message
        in the transmit window, i.e., it should refer to a message in
        relq.  (It's not always an error if it refers to a message outside
        the window - it is initialised that way - but it would be possible
        to extend this code to detect absurd rxack values.) */

        if(! msgq_match(relq, rxack))
                return;

        /* Finally, we walk relq, discarding its contents, until after
        we discard the packet with sequence number rxack. */

        while((m = msgq_pop(&relq)) != (TXMSG*)(NULL)) {
                seq = m->seq;
                ABCSP_TXMSG_DONE(m->m);
                DELETE(m); 
                if(seq == rxack)
                        break;
                }
}


/****************************************************************************
NAME
        get_txmsgbuf  -  get a block of transmit message bytes

FUNCTION
        Obtains a block of bytes from the currently selected transmit
        message.

RETURNS
        The base of the byte buffer to be transmitted, or NULL if no
        (more) bytes remain to be sent.

        The length of any returned buffer is written at "*len".
*/

static
uint8 *get_txmsgbuf(uint16 *len)
{
        unsigned ulen;
        uint8 *ret;

        /* Take from a plain byte buffer if an internally-generated msg. */
        if(curr.type == CURRMSG_TYPE_INTMSG) {
                if(curr.m.inter.outdex < curr.len) {
                        *len = curr.len - curr.m.inter.outdex;
                        return( &(curr.m.inter.buf[curr.m.inter.outdex]));
                        }
                return((uint8*)(NULL));
                }

        /* Otherwise ask the environment for a buffer holding part
        of the whole message. */

        /* This should be one line, but lint moans. */
        ret = (uint8*)(ABCSP_TXMSG_GETBUF(curr.m.txmsg->m, &ulen));
        *len = (uint16)(ulen);
        return(ret);
}


/****************************************************************************
NAME
        txmsgbuf_taken  -  how many source bytes have been taken

FUNCTION
        Tells the current message source how many bytes have just been
        consumed.
*/

static
void txmsgbuf_taken(uint16 ntaken)
{
        if(curr.type == CURRMSG_TYPE_INTMSG)
                curr.m.inter.outdex += ntaken;
        else
                ABCSP_TXMSG_TAKEN(curr.m.txmsg->m, ntaken);
}


/****************************************************************************
NAME
        restart_timeout  -  stop and restart the bcsp timer

FUNCTION
        Cancels the current BCSP timed event timer, if it is running, then
        starts the BCSP timed timer.
*/

void restart_timeout(void)
{
        ABCSP_CANCEL_BCSP_TIMER();
        ABCSP_START_BCSP_TIMER();
}


/****************************************************************************
NAME
        slip_one_byte  -  slip one byte and send it to the uart

RETURNS
        TRUE if the byte "b" was slipped and sent to the UART, else FALSE.
*/

static
bool slip_one_byte(uint8 b)
{
        bool more;      /* value unused. */

        return((abcsp_txslip_sendbuf(&b, 1, &more) == 1) ? TRUE : FALSE);
}


/****************************************************************************
NAME
        msgq_pop_destroy  -  destroy a message from the base of a queue

FUNCTION
        Pops a message from the base of the message queue "q" and discards
        its resources.

RETURNS
        TRUE if a message was taken from the queue and discarded, else FALSE.
*/

static
bool msgq_pop_destroy(TXMSG **q)
{
        TXMSG *m;

        if((m = msgq_pop(q)) != (TXMSG*)(NULL)) {
                ABCSP_TXMSG_DONE(m->m);
                DELETE(m); 
                return(TRUE);
                }
        return(FALSE);
}


/****************************************************************************
NAME
        msgq_add  -  add a message to the end of a queue

FUNCTION
        Adds the message "m" to the end of the message queue "q".
*/

static
void msgq_add(TXMSG **q, TXMSG *m)
{
        TXMSG *pm;

        if(*q == (TXMSG*)(NULL))
                *q = m;
        else {
                for(pm = *q; pm->next != (TXMSG*)(NULL) ; pm = pm->next )
                        ;
                pm->next = m;
                }
}


/****************************************************************************
NAME
        msgq_pop  -  take a message from the base of a queue

RETURNS
        The message from the base of the message queue "q", or NULL if
        the queue is empty.
*/

static
TXMSG *msgq_pop(TXMSG **q)
{
        TXMSG *ret;

        if(*q == (TXMSG*)(NULL))
                return((TXMSG*)(NULL));

        ret = *q;
        *q = ret->next;

        return(ret);
}


/****************************************************************************
NAME
        msgq_match  -  match an entry in a message queue

RETURNS
        The message from the queue "q" that has sequence number "seq", or
        NULL if the message is not present.
*/

static
TXMSG *msgq_match(TXMSG *q, uint8 seq)
{
        if(q == (TXMSG*)(NULL))
                return((TXMSG*)(NULL));

        for(; q != (TXMSG*)(NULL); q = q->next)
                if(q->seq == seq)
                        return(q);

        return((TXMSG*)(NULL));
}


/****************************************************************************
NAME
        msgq_len  -  how many messages are in a queue

RETURNS
        The number of messages stored in the queue "q".
*/

static
uint16 msgq_len(TXMSG *q)
{
        uint16 i;

        for(i = 0; q != (TXMSG*)(NULL); q = q->next, i++)
                ;
        return(i);
}


/****************************************************************************
NAME
        incrmod8  -  increment, modulo 8
*/

static
uint8 incrmod8(uint8 n)
{
        return(++n & 0x07);
}


/****************************************************************************
NAME
        decrmod8  -  decrement, modulo 8
*/

static
uint8 decrmod8(uint8 n)
{
        return(--n & 0x07);
}
