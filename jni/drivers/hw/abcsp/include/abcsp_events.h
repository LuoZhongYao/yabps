/****************************************************************************
FILE
        abcsp_events.h  -  abcsp event codes

DESCRIPTION
	The macro ABCSP_EVENT() is used to route event information from the
	abcsp library to its environment.  This file defines the event codes
	used as arguments to ABCSP_EVENT().

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __ABCSP_EVENTS_H__
#define __ABCSP_EVENTS_H__
 
#ifdef RCS_STRINGS
static const char abcsp_events_h_id[]
  = "$Id: abcsp_events.h,v 1.1 2001/07/06 20:11:02 cjo Exp $";
#endif

/* Initialisation of the abcsp library has started. */
#define ABCSP_EVT_START          ((unsigned)(1))

/* Initialisation of the abcsp library has been completed. */
#define ABCSP_EVT_INITED         ((unsigned)(2))

/* The BCSP Link Establishment engine has established sync with its peer
and the choke has been removed locally.  Messages will now be accepted
by abcsp_sendmsg() and abcsp_uart_deliverbytes(). */
#define ABCSP_EVT_LE_SYNC        ((unsigned)(3))

/* The BCSP Link Establishment engine has received confirmation from the peer
that it's in its "garrulous" state, so allowing generation of the sync-lost
event below. */
#define ABCSP_EVT_LE_CONF        ((unsigned)(4))

/* The BCSP Link Establishment engine has detected that the peer BCSP-LE
engine has restarted.  This presumably means that the peer BCSP stack
(or system) has been restarted.  A common local response would be to
restart (abcsp_init()) the local BCSP stack. */
#define ABCSP_EVT_LE_SYNC_LOST   ((unsigned)(5))

/* An attempt has been made to use an uninitialised block of code.  This
should only mean that a call has been made to the abcsp library before
abcsp_init() has been called. */
#define ABCSP_EVT_UNINITED       ((unsigned)(6))

/* The rx SLIP engine has established sync on the received byte stream. */
#define ABCSP_EVT_SLIP_SYNC      ((unsigned)(7))

/* The rx SLIP engine has lost sync on the received byte stream. */
#define ABCSP_EVT_SLIP_SYNC_LOST ((unsigned)(8))

/* A message received by the BCSP stack has been discarded because the local
choke is applied.  This is a reasonable occurrence where the peer BCSP stack
has removed its choke (shortly) before the local choke is removed. */
#define ABCSP_EVT_RX_CHOKE_DISCARD ((unsigned)(9))

/* A message submitted to abcsp_sendmsg() has been rejected because the
local choke is applied. */
#define ABCSP_EVT_TX_CHOKE_DISCARD ((unsigned)(10))

/* A message submitted to abcsp_sendmsg() has been rejected because the
BCSP transmit window is full. */
#define ABCSP_EVT_TX_WINDOW_FULL_DISCARD ((unsigned)(11))

/* The header of a received message has declared its payload to be
longer than the limit set by the #define ABCSP_RXMSG_MAX_PAYLOAD_LEN
and the message is being discarded.  (See the #define's description
in config_rxmsg.h.) */
#define ABCSP_EVT_OVERSIZE_DISCARD ((unsigned)(12))

/* A received reliable message has been rejected because its sequence
number is not that expected by the local BCSP stack. */
#define ABCSP_EVT_MISSEQ_DISCARD ((unsigned)(13))

/* A received message's header checksum has failed and the message is
being discarded. */
#define ABCSP_EVT_CHECKSUM       ((unsigned)(14))

/* A received message held fewer bytes than its header declared. */
#define ABCSP_EVT_SHORT_PAYLOAD  ((unsigned)(15))

/* A received message held more bytes than its header declared. */
#define ABCSP_EVT_OVERRUN        ((unsigned)(16))

/* A received message is being discarded because its CRC check failed. */
#define ABCSP_EVT_CRC_FAIL       ((unsigned)(17))

/* A call to ABCSP_MALLOC() or ABCSP_ZMALLOC() has failed. */
#define ABCSP_EVT_MALLOC         ((unsigned)(18))


#endif  /* __ABCSP_EVENTS_H__ */
