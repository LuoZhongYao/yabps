/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __L2CAP_H__
#define __L2CAP_H__
#include <zl/types.h>
#include <acl.h>
__BEGIN_DECLS

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
    u16 src_cid;
} __packed l2cap_connection_request_t;

typedef struct {
    __SIGNALING;
    u16 dest_cid;
    u16 src_cid;
    u16 result;
    u16 status;
} __packed l2cap_connection_response_t;

typedef struct {
    __SIGNALING;
    u16 dest_cid;
    u16  cflag;
    u8  mtu_type;
    u8  mtu_length;
    u16 max_mtu;
} __packed l2cap_configure_request_t;

typedef struct {
    __SIGNALING;
    u16 src_cid;
    u16 cflag;
    u16 result;
} __packed l2cap_configure_response_t;

__END_DECLS
#endif

