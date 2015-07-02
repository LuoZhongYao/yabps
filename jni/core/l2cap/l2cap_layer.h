/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jul 02
 ************************************************/
#ifndef __L2CAP_LAYER_H__
#define __L2CAP_LAYER_H__
#include <zl/types.h>
#include <l2cap.h>

__BEGIN_DECLS

typedef struct {
    enum l2cap_state state;
    u16  scid;
    u16  dcid;
    u16  cid;
    u16  psm;
}l2cap_cbk_t;

typedef struct {
    enum l2cap_state state;
    u16 psm;
    int (*handler)(l2cap_cbk_t *cbk,void *args);
} l2cap_listen_t;

void register_l2cap_listen(const u16 psm,
        int (*handler)(l2cap_cbk_t *cbk,void *args));
l2cap_listen_t *find_l2cap_listen(const u16 psm);
l2cap_cbk_t *find_l2cap_cbk(const u16 cid);

__END_DECLS
#endif

