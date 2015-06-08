/****************************************************************************
FILE
        rxbcsp.c  -  receive a bcsp packet

CONTAINS
        abcsp_rxbcsp_init  -  initialise the message assembler
        abcsp_rxbcsp_putbyte  -  add a byte to a received message
        abcsp_rxbcsp_done  -  signal that message is complete
        abcsp_rxbcsp_destroy  -  abandon construction of received message

        hdr_len  -  bcsp header's payload length field
        hdr_chan  -  bcsp header's protocol channel field
        hdr_rel  -  bcsp header's reliable field
        hdr_crc_present  -  does the bcsp header's indicate a crc
        hdr_cs_valid  -  is the bcsp header's checksum valid
        hdr_seq  -  bcsp header's seq field
        hdr_ack  -  bcsp header's ack field

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
        1.2   23:apr:02  cjo    Acknowledge all received reliable packets,
				not just those expected.  Issue AB.2.
        1.3   23:apr:02  cjo    Don't report CRC failures on received rel
                                packets with the wrong seq.  Issue AB.3.
*/

#ifdef RCS_STRINGS
static const char rxbcsp_c_id[]
  = "$Id: rxbcsp.c,v 1.3 2002/04/23 16:46:42 cjo Exp $";
#endif

#include "abcsp_private.h"
#include "config_rxmsg.h"


/* Message assembler's state. */
enum rxstate_enum {
        rxstate_uninit,         /* Uninitialised. */
        rxstate_hdr1,           /* Waiting for 1st BCSP header byte. */
        rxstate_hdr2,           /* Waiting for 2nd BCSP header byte. */
        rxstate_hdr3,           /* Waiting for 3rd BCSP header byte. */
        rxstate_hdr4,           /* Waiting for last BCSP header byte. */
        rxstate_payload,        /* Message payload. */
        rxstate_crc1,           /* Waiting for 1st BCSP crc byte. */
        rxstate_crc2,           /* Waiting for 2nd BCSP crc byte. */
        rxstate_end,            /* End of BCSP message. */
        rxstate_misseq,         /* Eat and discard mis-sequenced message. */
        rxstate_consume         /* Eat and discard the message. */
        };
typedef enum rxstate_enum rxstate;
static rxstate state;

/* Receive BCSP header. */
static uint8 hdr[4];

#ifdef ABCSP_RXCRC

/* Receive BCSP crc field. */
static uint8 crc[2];

/* The CRC derived from the hdr and payload bytes of the current message. */
static uint16 message_crc;

#endif /* ABCSP_RXCRC */

/* Payload Field length counter. */
static uint16 payload_length;

/* Forward references. */
static bool xabcsp_rxbcsp_putbyte(uint8 c);
static bool hdr_rel(void);
static uint16 hdr_ack(void);
static uint16 hdr_seq(void);
static uint16 hdr_chan(void);
static uint16 hdr_len(void);
static bool hdr_cs_valid(void);
static bool hdr_crc_present(void);

#ifdef ABCSP_RXCRC
static uint16 crc_field(void);
#endif /* ABCSP_RXCRC */


/****************************************************************************
NAME
        abcsp_rxbcsp_init  -  initialise the message assembler
*/

void abcsp_rxbcsp_init(void)
{
        abcsp_rxbcsp_destroy();
        state = rxstate_hdr1;
}


/****************************************************************************
NAME
        abcsp_rxbcsp_putbyte  -  add a byte to a received message
*/

bool abcsp_rxbcsp_putbyte(uint8 c)
{
#ifdef ABCSP_RXCRC
        rxstate old_state = state;

        if(xabcsp_rxbcsp_putbyte(c)) {

                /* Update the message's CRC if the byte was accepted. */
                switch(old_state) {
                    case rxstate_hdr1:
                        abcsp_crc_init(&message_crc);
                        /* FALLTHROUGH. */
                    case rxstate_hdr2:
                    case rxstate_hdr3:
                    case rxstate_hdr4:
                    case rxstate_payload:
                    case rxstate_misseq:
                        if(hdr_crc_present())
                                abcsp_crc_update(&message_crc, c);
                        break;
                    default:
                        break;
                    }

                return(TRUE);
                }
        return(FALSE);
#else /* ABCSP_RXCRC */
        return(xabcsp_rxbcsp_putbyte(c));
#endif /* ABCSP_RXCRC */
}


static
bool xabcsp_rxbcsp_putbyte(uint8 c)
{
        /* The state machine proper. */
        switch(state) {
            default:
            case rxstate_uninit:
                ABCSP_EVENT(ABCSP_EVT_UNINITED);
                return(FALSE);
            case rxstate_hdr1:
                hdr[0] = c;
                state = rxstate_hdr2;
                break;
            case rxstate_hdr2:
                hdr[1] = c;
                state = rxstate_hdr3;
                break;
            case rxstate_hdr3:
                hdr[2] = c;
                state = rxstate_hdr4;
                break;
            case rxstate_hdr4:
                hdr[3] = c;
                if(!hdr_cs_valid()) {
                        ABCSP_EVENT(ABCSP_EVT_CHECKSUM);
                        state = rxstate_consume;
                        break;
                        }
                if(abcsp_txrx.choke && !(hdr_chan() == 1 && !hdr_rel())) {
                        ABCSP_EVENT(ABCSP_EVT_RX_CHOKE_DISCARD);
                        state = rxstate_consume;
                        break;
                        }
#ifdef ABCSP_RXMSG_MAX_PAYLOAD_LEN
                if(hdr_len() > ABCSP_RXMSG_MAX_PAYLOAD_LEN) {
                    ABCSP_EVENT(ABCSP_EVT_OVERSIZE_DISCARD);
                    state = rxstate_consume;
                    break;
                }
#endif
                if(hdr_rel() && hdr_seq() != abcsp_txrx.rxseq_txack) {
                    ABCSP_EVENT(ABCSP_EVT_MISSEQ_DISCARD);
                    state = rxstate_misseq;

                    /* BCSP must acknowledge all reliable packets to
                       avoid deadlock. */

                    abcsp_txrx.txack_req = 1;
                    ABCSP_REQ_PUMPTXMSGS();
                    break;
                }

                abcsp_rxmsgdemux_newmsg(hdr_chan(), hdr_rel(), hdr_len());

                if(hdr_len()) {
                        payload_length = 0;
                        state = rxstate_payload;
                        }
                else if(hdr_crc_present()) {
                        state = rxstate_crc1;
                        }
                else {
                        state = rxstate_end;
                        }
                break;
            case rxstate_payload:
                if(!abcsp_rxmsgdemux_putbyte(c))
                        return(FALSE);
                ++payload_length;

                if(payload_length == hdr_len()) {
                        if(hdr_crc_present())
                                state = rxstate_crc1;
                        else
                                state = rxstate_end;
                        }
                break;
            case rxstate_crc1:
#ifdef ABCSP_RXCRC
                crc[0] = c;
#endif /* ABCSP_RXCRC */
                state = rxstate_crc2;
                break;
            case rxstate_crc2:
#ifdef ABCSP_RXCRC
                crc[1] = c;
#endif /* ABCSP_RXCRC */
                state = rxstate_end;
                break;
            case rxstate_end:
                ABCSP_EVENT(ABCSP_EVT_OVERRUN);
                state = rxstate_consume;
                break;
            case rxstate_consume:
            case rxstate_misseq:
                break;
            }

        /* Signal byte consumed and another byte wanted. */
        return(TRUE);
}


/****************************************************************************
NAME
        abcsp_rxbcsp_done  -  signal that message is complete

IMPLEMENTATION NOTES
        This should be called at the end of receiving a bcsp message - the
        call will probably be triggered by the bcsp frame's trailing 0xc0.
        If all is well:

          -  this file's "state" variable should be rxstate_end, implying
                    the received header cs and payload length were correct,
          -  if a crc was present then the crc sums should be up to date,
          -  if the msg was rel then its seq number should match
                        abcsp_txrx.rxseq_txack

        If the length and cs are correct (we're in rxstate_end) and any crc
        is intact then we accept the ack, regardless of seq and rel/unrel.
*/

void abcsp_rxbcsp_done(void)
{
        switch(state) {
            case rxstate_payload:
                /* Check we've received the right number of bytes. */
                ABCSP_EVENT(ABCSP_EVT_SHORT_PAYLOAD);
                abcsp_rxbcsp_destroy();
                break;
            case rxstate_end:
            case rxstate_misseq:
#ifdef ABCSP_RXCRC
                /* Check the received message's crc, if present. */
                if(hdr_crc_present()
                          && (abcsp_crc_reverse(message_crc) != crc_field())) {
			if(state == rxstate_end)
                        	ABCSP_EVENT(ABCSP_EVT_CRC_FAIL);
                        abcsp_rxbcsp_destroy();
                        }
                else {
#endif /* ABCSP_RXCRC */
                        /* The packet seems to be intact. */
                        if(state == rxstate_end) {

                                /* We don't deliver ack msgs (chan zero). */
                                if(hdr_chan()) {

                                        /* Deliver the message. */
                                        abcsp_rxmsgdemux_done();
                                        }

                                /* If the message is reliable we need to note
                                the next rel rxseq we will accept.  This is
                                numerically identical to the value that we
                                send back to the peer in outbound packets'
                                ack fields to tell the host that we've got
                                this message, i.e., the ack value sent to the
                                peer is one more than the packet being
                                acknowledged, modulo 8. */

                                if(hdr_rel()) {
                                        ++abcsp_txrx.rxseq_txack;
                                        abcsp_txrx.rxseq_txack %= 0x8;
                                        abcsp_txrx.txack_req = 1;

                                        /* Wake the tx path to send the new
                                        ack val back to the peer. */

                                        ABCSP_REQ_PUMPTXMSGS();
                                        }
                                }

                        /* We accept rxack acknowledgement info from any
                        intact packet, reliable or unreliable.  This includes
                        reliable messages with the wrong seq number. */

                        if(hdr_ack() != abcsp_txrx.rxack) {
                                abcsp_txrx.rxack = hdr_ack();

                                /* Wake the tx path so that it can discard the
                                acknowledged message(s). */

                                ABCSP_REQ_PUMPTXMSGS();
                                }
#ifdef ABCSP_RXCRC
                        }
#endif /* ABCSP_RXCRC */
                break;
            default:
                break;
            }

        state = rxstate_hdr1;
}


/****************************************************************************
NAME
        abcsp_rxbcsp_destroy  -  abandon construction of received message
*/

void abcsp_rxbcsp_destroy(void)
{
        abcsp_rxmsgdemux_destroy();
        state = rxstate_hdr1;
}


/****************************************************************************
NAME
        hdr_rel  -  bcsp header's reliable field

RETURNS
        TRUE if the current BCSP header indicates that the packet is
        being passed on a reliable channel, else FALSE.
*/

static
bool hdr_rel(void)
{
        return(((hdr[0] & 0x80) == 0x80 ) ? TRUE : FALSE );
}


/****************************************************************************
NAME
        hdr_crc_present  -  does the bcsp header's indicate a crc

RETURNS
        TRUE if the current BCSP header indicates that the packet carries
        a CRC, else FALSE.
*/

static
bool hdr_crc_present(void)
{
        return(((hdr[0] & 0x40) == 0x40 ) ? TRUE : FALSE );
}


/****************************************************************************
NAME
        hdr_ack  -  bcsp header's ack field

RETURNS
        The content of the current BCSP header's Ack field.
*/

static
uint16 hdr_ack(void)
{
        return((hdr[0] >> 3) & 0x07);
}


/****************************************************************************
NAME
        hdr_seq  -  bcsp header's seq field

RETURNS
        The content of the current BCSP header's Seq field.
*/

static
uint16 hdr_seq(void)
{
        return(hdr[0] & 0x07);
}


/****************************************************************************
NAME
        hdr_chan  -  bcsp header's protocol channel field

RETURNS
        The Protocol Id field from the current BCSP header.
*/

static
uint16 hdr_chan(void)
{
        return((uint16)(hdr[1]) & 0x0f);
}


/****************************************************************************
NAME
        hdr_len  -  bcsp header's payload length field

RETURNS
        The content of the current BCSP header's Payload Length field.
*/

static
uint16 hdr_len(void)
{
        return(((hdr[1] >> 4) & 0x0f) + ((uint16)(hdr[2]) << 4));
}


/****************************************************************************
NAME
        hdr_cs_valid  -  is the bcsp header's checksum valid

RETURNS
        TRUE if the current BCSP header's Checksum field matches the
        rest of the header, else FALSE.
*/

static
bool hdr_cs_valid(void)
{
        return(((0xff & ~(hdr[0] + hdr[1] + hdr[2])) == hdr[3])
                        ? TRUE : FALSE );
}


#ifdef ABCSP_RXCRC

/****************************************************************************
NAME
        crc_field  -  value from the message's crc field

RETURNS
        The value of the BCSP packet's CRC field.
*/

static
uint16 crc_field(void)
{
        return((crc[0] << 8 ) + crc[1]);
}

#endif /* ABCSP_RXCRC */
