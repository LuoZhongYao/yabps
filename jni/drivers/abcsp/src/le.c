/****************************************************************************
FILE
        le.c  -  bcsp link establishment

CONTAINS
        abcsp_bcsple_init  -  initialise the link establishment engine
        abcsp_bcsple_newmsg  -  create a bcsp-le message
        abcsp_bcsple_putbyte  -  add a byte to a received bcsp-le message
        abcsp_bcsple_flush  -  push outstanding bytes back to bcsp-le message
        abcsp_bcsple_done  -  signal that bcsp-le message is complete
        abcsp_bcsple_destroy  -  abandon construction of bcsp-le message
        abcsp_tshy_timed_event  -  report a tshy timeout event to the fsm
        abcsp_tconf_timed_event  -  report a tconf timeout event to the fsm

        report_tshy_timeout  -  report a tshy timeout event to the fsm
        report_tconf_timeout  -  report a tconf timeout event to the fsm
        abcsp_le_fsm  -  the bcsp link establishment entity state machine
        match_lemsgid  -  match a link establishment message
        req_bcsple_msg  -  request transmission of a bcsp-le message

DESCRIPTION
        This file holds a state machine that implements the BCSP Link
        Establishment protocol.  There are two versions of the protocol:

          - The old version is described in CSR internal document
            bc01-s-010f, (known outside CSR as document AN105).

          - The new version is described in CSR internal document
            bc01-s-010g.  The only significant changes between the old and
            new versions of the protocol are:

                - the choke is now turned off when moving from "curious" to
                  "garrulous".

                - the "conf_cnt" counter, limiting the number of "conf"
                  messages sent to the peer, has been removed.  The state
                  machine always emits "conf" messages regularly in the
                  "curious" state.

        The old and new versions of the protocol should interoperate with all
        CSR firmware versions from beta-10 (March 2001).  Almost nobody is
        still using older firmware builds, as they were written against the
        old Bluetooth specification version 1.0b.

        The abcsp code can be configured to use the old protocol by
        #defining ABCSP_USE_OLD_BCSP_LE.

        In HCI 15.X firmware builds PSKEY_USE_OLD_BCSP_LE can force use of
        the older protocol.

IMPLEMENTATION NOTE
        The code doesn't generally bother to cancel timers - they time
        out harmlessly, so there's no point.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created from a heavily hacked copy of the
                                bc01 firmware's implementation.
        1.2   15:feb:02  cjo    Convert to new protocol definition, given
                                in bc01-s-010g.
*/

#ifdef RCS_STRINGS
static const char le_c_id[]
  = "$Id: le.c,v 1.2 2002/02/15 21:18:28 cjo Exp $";
#endif

#include <string.h>
#include "abcsp_private.h"
#include "config_le.h"


/* BCSP-LE state machine states. */
enum bcsp_le_state_enum {
        state_shy,
        state_curious,
        state_garrulous
        };
typedef enum bcsp_le_state_enum bcsp_le_state;


/* LE message identifiers - signals sent to/from bcsp-le state machines. */
enum lemsgid_enum {
        lemsgid_none,           /* no message */
        lemsgid_sync,           /* sync message */
        lemsgid_sync_resp,      /* sync-resp message */
        lemsgid_conf,           /* conf message */
        lemsgid_conf_resp,      /* conf-resp message */
        lemsgid_tshy_timeout,   /* message indicating Tshy timeout */
        lemsgid_tconf_timeout   /* message indicating Tconf timeout */
        };
typedef enum lemsgid_enum lemsgid;

/* Messages sent to/from the peer bcsp-le state machine.  All messages
are the same length, simplifying some of the code.   Messages are global
as they are used by the receive and transmit paths*/

#define BCSPLE_MSGLEN           (4)

const uint8 abcsp_le_sync_msg[]     = { 0xda, 0xdc, 0xed, 0xed };
const uint8 abcsp_le_syncresp_msg[] = { 0xac, 0xaf, 0xef, 0xee };
const uint8 abcsp_le_conf_msg[]     = { 0xad, 0xef, 0xac, 0xed };
const uint8 abcsp_le_confresp_msg[] = { 0xde, 0xad, 0xd0, 0xd0 };

/* Database mapping messages sent to/from the peer bcsp-le state machine
to message identifiers. */

typedef struct {
        lemsgid         id;     /* Message identifier. */
        const uint8     *msg;   /* The message itself. */
        } LEMSG;

static const LEMSG lemsgs[] = {
        { lemsgid_sync,           abcsp_le_sync_msg },
        { lemsgid_sync_resp,      abcsp_le_syncresp_msg },
        { lemsgid_conf,           abcsp_le_conf_msg },
        { lemsgid_conf_resp,      abcsp_le_confresp_msg },
        { lemsgid_none,           ( const uint8* )( NULL ) }
        };


static bcsp_le_state state;
static uint8 msgbuf[BCSPLE_MSGLEN];
static uint8 msgbuf_index;

#ifdef ABCSP_USE_OLD_BCSP_LE
static uint8 conf_cnt;
#endif


/* Forward references. */
static void abcsp_lm_fsm(lemsgid msg);
static lemsgid match_lemsgid(const uint8 *msg);
static void req_bcsple_msg(lemsgid id);


/****************************************************************************
NAME
        abcsp_bcsple_init  -  initialise the link establishment entity
*/

void abcsp_bcsple_init(void)
{
        /* Attempt to prevent any existing timed events. */
        ABCSP_CANCEL_TCONF_TIMER();
        ABCSP_CANCEL_TSHY_TIMER();

        /* Discard any BCSP-LE message being constructed. */
        abcsp_bcsple_destroy();

        /* Configure the initial state of the bcsp-le state machine. */
#ifdef ABCSP_USE_BCSP_LE_PASSIVE_START
        abcsp_txrx.bcsple_muzzled = TRUE;
#else
        abcsp_txrx.bcsple_muzzled = FALSE;
#endif

        /* Stop most BCSP traffic flowing. */
        abcsp_txrx.choke = TRUE;

        /* Arrange to be called after a respectful interval. */
        ABCSP_START_TSHY_TIMER();

        /* Emit the first sync message if not using passive-start. */
        if(!abcsp_txrx.bcsple_muzzled)
                req_bcsple_msg(lemsgid_sync);

        /* State machine's initial state. */
        state = state_shy;
}


/****************************************************************************
NAME
        abcsp_bcsple_newmsg  -  create a bcsp-le message
*/

bool abcsp_bcsple_newmsg(void)
{
        /* Discard any existing message. */
        abcsp_bcsple_destroy();

        return(TRUE);
}


/****************************************************************************
NAME
        abcsp_bcsple_putbyte  -  add a byte to a received bcsp-le message
*/

bool abcsp_bcsple_putbyte(uint8 c)
{
        /* Just add new byte to buffer if there's room. */
        if(msgbuf_index < BCSPLE_MSGLEN)
                msgbuf[msgbuf_index++] = c;

        /* If the BCSP-LE message has been corrupted then we may receive a 5+
        byte message.  We always reply TRUE so as not to block the whole of
        the receive path - we just accept and discard the extra bytes. */
        return(TRUE);
}


/****************************************************************************
NAME
        abcsp_bcsple_flush  -  push outstanding bytes back to bcsp-le message
*/

void abcsp_bcsple_flush(void)
{
        /* Stub. */
}


/****************************************************************************
NAME
        abcsp_bcsple_done  -  signal that bcsp-le message is complete
*/

void abcsp_bcsple_done(void)
{
        abcsp_lm_fsm(match_lemsgid((const uint8*)(msgbuf)));

        /* Overwrite the old message. */
        abcsp_bcsple_destroy();
}


/****************************************************************************
NAME
        abcsp_bcsple_destroy  -  abandon construction of bcsp-le message
*/

void abcsp_bcsple_destroy(void)
{
        /* Wipe out any partially-constructed BCSP-LE message. */
        msgbuf_index = 0;

        /* Overwrite the old message. */
        msgbuf[0] = 0x00;
        msgbuf[1] = 0x00;
        msgbuf[2] = 0x00;
        msgbuf[3] = 0x00;
}


/****************************************************************************
NAME
        abcsp_tshy_timed_event  -  report a tshy timeout event to the fsm
*/

void abcsp_tshy_timed_event(void)
{
        abcsp_lm_fsm(lemsgid_tshy_timeout);
}


/****************************************************************************
NAME
        abcsp_tconf_timed_event  -  report a tconf timeout event to the fsm
*/

void abcsp_tconf_timed_event(void)
{
        abcsp_lm_fsm(lemsgid_tconf_timeout);
}


/****************************************************************************
NAME
        abcsp_lm_fsm  -  the bcsp link establishment entity state machine

FUNCTION
        The message "msg" is fed into the bcsp-le state machine.
*/

static
void abcsp_lm_fsm(lemsgid msg)
{
        switch(state) {
            case state_shy:
                switch(msg) {
                    case lemsgid_tshy_timeout:
                        if(!abcsp_txrx.bcsple_muzzled)
                                req_bcsple_msg(lemsgid_sync);
                        ABCSP_START_TSHY_TIMER();
                        break;
                    case lemsgid_sync:
                        abcsp_txrx.bcsple_muzzled = FALSE;
                        req_bcsple_msg(lemsgid_sync_resp);
                        break;
                    case lemsgid_sync_resp:
                        ABCSP_EVENT(ABCSP_EVT_LE_SYNC);
#ifdef ABCSP_USE_OLD_BCSP_LE
                        abcsp_txrx.choke = FALSE;
                        conf_cnt = 0;
#else
                        req_bcsple_msg(lemsgid_conf);
#endif
                        ABCSP_START_TCONF_TIMER();
                        state = state_curious;
                        break;
                    default:
                        break;
                    }
                break;
            case state_curious:
                switch(msg) {
                    case lemsgid_tconf_timeout:
#ifdef ABCSP_USE_OLD_BCSP_LE
                        if(conf_cnt < ABCSP_USE_BCSP_LE_CONF_CNT_LIMIT) {
                                ++conf_cnt;
                                req_bcsple_msg(lemsgid_conf);
                                ABCSP_START_TCONF_TIMER();
                                }
#else
                        req_bcsple_msg(lemsgid_conf);
                        ABCSP_START_TCONF_TIMER();
#endif
                        break;
                    case lemsgid_sync:
                        req_bcsple_msg(lemsgid_sync_resp);
                        break;
                    case lemsgid_conf:
                        req_bcsple_msg(lemsgid_conf_resp);
                        break;
                    case lemsgid_conf_resp:
                        ABCSP_EVENT(ABCSP_EVT_LE_CONF);
#ifndef ABCSP_USE_OLD_BCSP_LE
                        abcsp_txrx.choke = FALSE;
#endif
                        state = state_garrulous;
                        break;
                    default:
                        break;
                    }
                break;
            case state_garrulous:
                switch(msg) {
                    case lemsgid_conf:
                        req_bcsple_msg(lemsgid_conf_resp);
                        break;
                    case lemsgid_sync:
                        /* Peer has apparently restarted. */
                        ABCSP_EVENT(ABCSP_EVT_LE_SYNC_LOST);
                        break;
                    }
                break;
            default:
                break;
            }
}


/****************************************************************************
NAME
        match_lemsgid  -  match a link establishment message

RETURNS
        An identifier for the bcsp-le wire message at "msg", or
        lemsgid_none if the message could not be recognised.
*/

static
lemsgid match_lemsgid(const uint8 *msg)
{
        const LEMSG *m;

        /* The message should be 4 bytes long.  If so, match this against
        the expected message byte patterns and deliver any corresponding
        token to the bcsp-le state machine. */

        if(msgbuf_index == BCSPLE_MSGLEN)
                for(m = lemsgs ; m->msg != (const uint8*)(NULL) ; m++)
                        if(memcmp(msg, m->msg, BCSPLE_MSGLEN) == 0)
                                return(m->id);

        return(lemsgid_none);
}


/****************************************************************************
NAME
        req_bcsple_msg  -  request transmission of a bcsp-le message

FUNCTION
        Requests the transmit path of the abcsp library to emit one
        of the (fixed content) bcsp-le messages.
*/

static
void req_bcsple_msg(lemsgid id)
{
        /* <Sigh.>  The bitfield abcsp_txrx means we can't table-drive. */
        switch(id) {
            case lemsgid_sync:
                abcsp_txrx.txsync_req = 1;
                break;
            case lemsgid_sync_resp:
                abcsp_txrx.txsyncresp_req = 1;
                break;
            case lemsgid_conf:
                abcsp_txrx.txconf_req = 1;
                break;
            case lemsgid_conf_resp:
                abcsp_txrx.txconfresp_req = 1;
                break;
            default:
                return;
            }

        /* Kick the transmit path into wakefulness. */
        ABCSP_REQ_PUMPTXMSGS();
}
