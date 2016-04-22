/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jul 02
 ************************************************/
#ifndef __L2CAP_LAYER_H__
#define __L2CAP_LAYER_H__
#include <zl/types.h>
#include <l2cap.h>

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

/* Configuration types */
#define L2CAP_CFG_MTU           (0x01)
#define L2CAP_CFG_FLUSHTO       (0x02)
#define L2CAP_CFG_QOS           (0x03)
#define L2CAP_CFG_FCR           (0x04)
#define L2CAP_CFG_FCS           (0x05)
#define L2CAP_CFG_EXT_FLOW      (0x06)
#define L2CAP_CFG_EXT_WIN_SIZE  (0x07)


/* Configuration types length */
#define L2CAP_CFG_MTU_LEN               (2)           /* MTU option length    */
#define L2CAP_CFG_FLUSH_LEN             (2)           /* Flush option len     */
#define L2CAP_CFG_QOS_LEN               (22)          /* QOS option length    */
#define L2CAP_CFG_FCR_LEN               (9)           /* FCR option length    */
#define L2CAP_CFG_FCS_LEN               (1)           /* FCR option length    */
#define L2CAP_CFG_EXT_FLOW_LEN          (16)          /* Extended Flow Spec   */
#define L2CAP_CFG_EXT_WIN_SIZE_LEN      (2)           /* Ext window size length */
#define L2CAP_CFG_OVERHEAD              (2)           /* Type and length      */

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
    L2CAP_CONNECT_REQ,
    L2CAP_CONNECT_RSP,
    L2CAP_CONFIG,
    L2CAP_OPEN,
    L2CAP_DISCONNECT_REQ,
    L2CAP_DISCONNECT_RSP,
};

#define __SIGNALING u8 code;u8 identifier;u16 length

void *alloc_l2cap_packed(l2cap_t **l2cap,u16 length);

typedef struct {
    __SIGNALING;
    u8 payload[0];
} __packed l2cap_signaling_t;

void l2cap_signaling_handler(u16 handle,l2cap_signaling_t *sig);
void *alloc_l2cap_signaling_packed(l2cap_signaling_t**sig,u16 length);

typedef struct {
    __SIGNALING;
    u16 psm;
    u16 remote_cid;   /* remote cid */
} __packed l2cap_connection_request_t;

typedef struct {
    __SIGNALING;
    u16 local_cid;   /* local cid */
    u16 remote_cid;   /* remote cid */
    u16 result;
    u16 status;
} __packed l2cap_connection_response_t;

typedef struct {
    u8 type;
    u8 length;
    u8 data[0];
}__packed l2cap_config_option_t;

typedef struct {
    __SIGNALING;
    u16 local_cid;   /* local cid */
    u16 cflag;
    l2cap_config_option_t option[0];
} __packed l2cap_configure_request_t;


typedef struct {
    __SIGNALING;
    u16 remote_cid;   /* remote cid */
    u16 cflag;
    u16 result;
    l2cap_config_option_t option[0];
} __packed l2cap_configure_response_t;

typedef struct {
    __SIGNALING;
    u16 info_type;
} __packed l2cap_information_request_t;

typedef struct {
    __SIGNALING;
    u16 info_type;
    u16 result;
    u8  data[0];
} __packed l2cap_information_response_t;

/********************************************************/

struct l2cap_cbk_t {
    enum l2cap_state state;
    u16  remote_cid;        /* remote cid */
    u16  local_cid;         /* local cid */
    u16  psm;
    l2cap_task task;
    void *private_data;
};

struct l2cap_protocol{
    u16 psm;
    l2cap_task task;
} ;


extern l2cap_cbk_t *l2cap_alloc_cbk(u16 cid, enum l2cap_state status,l2cap_task task);
extern l2cap_cbk_t *find_l2cap_cbk(const u16 cid);
extern struct l2cap_protocol *find_protocol_psm(u16 psm);

extern void l2cap_cbk_free(l2cap_cbk_t *cbk);

__END_DECLS
#endif

