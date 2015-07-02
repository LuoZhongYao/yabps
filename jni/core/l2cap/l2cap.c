#include <zl/types.h>
#include <zl/log.h>
#include <zl/util.h>
#include <l2cap.h>
#include <acl.h>
#include <hci.h>
#include "l2cap_layer.h"
#include <assert.h>

#ifdef __LOG_L2CAP__
#define L2C_LOGD(fmt,...)   LOGD("[L2C] "fmt,##__VA_ARGS__)
#else
#define L2C_LOGD(...)
#endif

int l2cap_send(l2cap_t *l2cap,u16 handle)
{
    u16 len,pos;
    const u16 maxsize = 1024;
    u8 flag = 0x02;
    acl_t *acl = packet_of(acl_t,l2cap);
    acl->handle = handle;
    len = l2cap->length + sizeof(*l2cap);
    pos = 0;

    while(len) {
        if(len > maxsize) {
            acl->flags = flag;
            acl->length = maxsize;

            hci_send(CHANNEL_ACL,acl,acl->length + sizeof(*acl));

            pos += maxsize;
            len -= maxsize;

            memcpy(acl->payload,acl->payload + pos,len);
            flag = 0x01;
        } else {
            acl->flags = flag;
            acl->length = len;
            hci_send(CHANNEL_ACL,acl,acl->length + sizeof(*acl));
            break;
        };
    }
    return 0;
} 

void *alloc_l2cap_packed(l2cap_t **l2cap,u16 length)
{
    acl_t *acl;
    acl = alloc_acl_packed(sizeof(l2cap_t) + length);
    *l2cap = (void*)acl->payload;
    return acl;
}

void l2cap_handler(l2cap_t *l2cap)
{
    L2C_LOGD("cid %x",l2cap->cid);
    switch(l2cap->cid) {
    case L2CAP_SIG_CID:
        l2cap_signaling_handler((l2cap_signaling_t*)l2cap->payload);
        break;
    case L2CAP_CONNLESS_CID:
        break;
    default:
        {
            l2cap_cbk_t *l2c = find_l2cap_cbk(l2cap->cid);
            if(l2c) {
                L2C_LOGD("state %x",l2c->state);
            } else {
                L2C_LOGD("Unhandle cid %x",l2cap->cid);
            }
        }
        break;
    }
    __test_delete(l2cap);
}
