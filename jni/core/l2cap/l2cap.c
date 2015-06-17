#include <zl/types.h>
#include <zl/log.h>
#include <zl/util.h>
#include <l2cap.h>
#include <acl.h>
#include <hci.h>
#include <assert.h>

#define acl_by_l2cap(s)   ({\
    const void *__mptr = (s);\
    (acl_t *)((u8 *)__mptr - offsetof(acl_t,payload));})

int l2cap_send(l2cap_t *l2cap,u16 handle,u8 flags)
{
    acl_t *acl = packet_of(acl_t,l2cap);
    acl->handle = handle;
    acl->flags = flags;
    acl->length = l2cap->length + sizeof(*l2cap);
    return hci_send(CHANNEL_ACL,acl,sizeof(*acl) + acl->length);
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
    switch(l2cap->cid) {
    case 1:
        l2cap_signaling_handler((l2cap_signaling_t*)l2cap->payload);
        break;
    case 2:
        break;
    }
    __test_delete(l2cap);
}
