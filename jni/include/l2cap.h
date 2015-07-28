/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __L2CAP_H__
#define __L2CAP_H__
#include <zl/types.h>
#include <acl.h>
__BEGIN_DECLS

/* Signal codes */
#define L2CAP_CMD_REJ       (0x01)
#define L2CAP_CONN_REQ      (0x02)
#define L2CAP_CONN_RSP      (0x03)
#define L2CAP_CFG_REQ       (0x04)
#define L2CAP_CFG_RSP       (0x05)
#define L2CAP_DISCONN_REQ   (0x06)
#define L2CAP_DISCONN_RSP   (0x07)
#define L2CAP_ECHO_REQ      (0x08)
#define L2CAP_ECHO_RSP      (0x09)
#define L2CAP_INFO_REQ      (0x0A)
#define L2CAP_INFO_RSP      (0x0B)

/* permanent channel ideentifiers */
#define L2CAP_NULL_CID      (0x0000)
#define L2CAP_SIG_CID       (0x0001)
#define L2CAP_CONNLESS_CID  (0x0002)

/* channel identifiers values */
#define L2CAP_MIN_CID       (0x0040)
#define L2CAP_MAX_CID       (0xffff)

/* Protocol and service multiplexor */
#define SDP_PSM             (0x0001)
#define RFCOMM_PSM          (0x0003)
#define BNEP_PSM            (0x000F)

/* Configuration types */
#define L2CAP_CFG_MTU       (0x01)
#define L2CAP_FLUSHTO       (0x02)
#define L2CAP_QOS           (0x030

/* Configuration types length */
#define L2CAP_MTU_LEN       (2)
#define L2CAP_FLUSHTO_LEN   (2)
#define L2CAP_QOS_LEN       (22)

/* Configuration response types */
#define L2CAP_CFG_SUCCESS   (0x0000)
#define L2CAP_CFG_UNACCEPT  (0x0001)
#define L2CAP_CFG_REJ       (0x0002)
#define L2CAP_CFG_UNKNOWN   (0x0003)
#define L2CAP_CFG_TIMEOUT   (0xEEEE0

/* QoS types */
#define L2CAP_QOS_NO_TRAFFIC    (0x00)
#define L2CAP_QOS_BEST_EFFORT   (0x01)
#define L2CAP_QOS_GUARANTEED    (0x02)

/* Command reject reasons */
#define L2CAP_CMD_NOT_UNDERSTOOD    0x0000
#define L2CAP_MTU_EXCEEDED          0x0001
#define L2CAP_INVALID_CID           0x0002

/* Connection response results */
#define L2CAP_CONN_SUCCESS  0x0000
#define L2CAP_CONN_PND      0x0001
#define L2CAP_CONN_REF_PSM  0x0002
#define L2CAP_CONN_REF_SEC  0x0003
#define L2CAP_CONN_REF_RES  0x0004
#define L2CAP_CONN_CFG_TO   0x0005 /* Implementation specific result */

/* Echo response results */
#define L2CAP_ECHO_RCVD     0x00
#define L2CAP_ECHO_TO       0x01

/* L2CAP segmentation */
#define L2CAP_ACL_START     0x02
#define L2CAP_ACL_CONT      0x01

/* L2CAP config default parameters */
#define L2CAP_CFG_DEFAULT_INMTU 672 /* Two Baseband DH5 packets (2*341=682) minus the Baseband ACL 
				       headers (2*2=4) and L2CAP header (6) */
#define L2CAP_CFG_DEFAULT_OUTFLUSHTO 0xFFFF

/* L2CAP configuration parameter masks */
#define L2CAP_CFG_IR            0x01
#define L2CAP_CFG_IN_SUCCESS    0x02
#define L2CAP_CFG_OUT_SUCCESS   0x04
#define L2CAP_CFG_OUT_REQ       0x08

enum l2cap_state {
    L2CAP_CLOSED,
    L2CAP_LISTEN,
    L2CAP_CONNECT_RSP,
    L2CA_CONNECT_RSP,
    L2CAP_CONFIG,
    L2CAP_OPEN,
    L2CAP_DISCONNECT_RSP,
    L2CA_DISCONNECT_RSP,
};

#define __SIGNALING u8 code;u8 identifier;u16 length

typedef struct {
    u16 length;
    u16 cid;
    u8  payload[0];
} __packed l2cap_t;

int l2cap_send(l2cap_t *l2cap,u16 handle);
void l2cap_handler(l2cap_t *l2cap);
void *alloc_l2cap_packed(l2cap_t **l2cap,u16 length);

typedef struct {
    __SIGNALING;
    u8 payload[0];
} __packed l2cap_signaling_t;

void l2cap_signaling_handler(l2cap_signaling_t *sig);
void *alloc_l2cap_signaling_packed(l2cap_signaling_t**sig,u16 length);

typedef struct {
    __SIGNALING;
    u16 psm;
    u16 scid;
} __packed l2cap_connection_request_t;

typedef struct {
    __SIGNALING;
    u16 dcid;
    u16 scid;
    u16 result;
    u16 status;
} __packed l2cap_connection_response_t;

typedef struct {
    __SIGNALING;
    u16 dcid;
    u16 cflag;
    u8  mtu_type;
    u8  mtu_length;
    u16 max_mtu;
} __packed l2cap_configure_request_t;

typedef struct {
    __SIGNALING;
    u16 scid;
    u16 cflag;
    u16 result;
} __packed l2cap_configure_response_t;

__END_DECLS
#endif

