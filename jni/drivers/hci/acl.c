#include <acl.h>
#include <l2cap.h>
#include <hci.h>
#include <zl/log.h>
#include <zl/util.h>
#include <assert.h>

#ifdef __LOG_ACL__
#define ACL_LOGD(fmt,...)   LOGD("[ACL] "fmt,##__VA_ARGS__)
#else
#define ACL_LOGD(...)
#endif

acl_t *alloc_acl_packed(u16 length) {
    acl_t *acl = malloc(sizeof(*acl) + length);
    assert(acl);
    return acl;
}

void acl_handler(acl_t *acl)
{
    blk_t *blk;
    l2cap_t *l2cap;
    u16 length;
    blk = get_blk(acl->handle,hci_blks,MAX_ACL_CONNECTIONS);
    if(blk == NULL) {
        LOGE("Can't found acl counnection");
        return;
    }

    ACL_LOGD("handle %x,flags %x",acl->handle,acl->flags);
    if(acl->flags == 0x2) {
        l2cap = (void*)acl->payload;
        __test_delete(blk->blk);
        blk->total = l2cap->length;
        blk->blk = malloc(sizeof(*l2cap) + blk->total);
        assert(blk->blk);
        if(acl->length <= blk->total + sizeof(*l2cap))
            length = acl->length;
        else
            length = blk->total + sizeof(*l2cap);
        memcpy(blk->blk,acl->payload,length);
        blk->length = length - sizeof(*l2cap);
        blk->handle = acl->handle;
    } else {
        if(blk->blk != NULL) {
            if(blk->length + acl->length <= blk->total) 
                length = acl->length;
            else
                length = blk->total - blk->length;
            memcpy(blk->blk + blk->length + sizeof(*l2cap),acl->payload,length);
            blk->length += length;
        }
    }

    ACL_LOGD("cid %x,length %x",l2cap->cid,l2cap->length);

    if(blk->length >= blk->total) {
        l2cap = blk->blk;
        blk->blk = NULL;
        blk->length = 0;
        l2cap_handler(l2cap);
    }
}
