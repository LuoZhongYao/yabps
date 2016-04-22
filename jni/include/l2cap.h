/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __L2CAP_H__
#define __L2CAP_H__
#include <zl/types.h>
#include <acl.h>
__BEGIN_DECLS

/* Protocol and service multiplexor */
#define SDP_PSM             (0x0001)
#define RFCOMM_PSM          (0x0003)
#define BNEP_PSM            (0x000F)

typedef struct {
    u16 length;
    u16 cid;
    u8  payload[0];
} __packed l2cap_t;


typedef struct l2cap_cbk_t l2cap_cbk_t;
typedef void (*l2cap_task)(l2cap_cbk_t *cbk,l2cap_t *l2c);

extern int l2cap_send(u16 handle,l2cap_t *l2cap);
extern void l2cap_handler(u16 handle,l2cap_t *l2cap);
extern void l2cap_register_protocol(u16 psm,l2cap_task task);

__END_DECLS
#endif

