#define TAG "ACL"

#include <acl.h>
#include <l2cap.h>
#include <hci.h>
#include <zl/log.h>
#include <zl/util.h>
#include "hci_layer.h"
#include <assert.h>

acl_t *alloc_acl_packed(u16 length) {
    acl_t *acl = malloc(sizeof(*acl) + length);
    assert(acl);
    return acl;
}

void acl_handler(acl_t *acl)
{
    hci_cbk_t *cbk;
    l2cap_t *l2cap;
    u16 length;
    cbk = find_hci_cbk(acl->handle);
    if(cbk == NULL) {
        LOGE("Can't found acl connection");
        return;
    }

    LOGD("handle %x,flags %x",acl->handle,acl->flags);
    if(acl->flags == 0x2) {
        l2cap = (void*)acl->payload;
        __test_delete(cbk->blk);
        cbk->total = l2cap->length;
        cbk->blk = malloc(sizeof(*l2cap) + cbk->total);
        assert(blk->blk);
        if(acl->length <= cbk->total + sizeof(*l2cap))
            length = acl->length;
        else
            length = cbk->total + sizeof(*l2cap);
        memcpy(cbk->blk,acl->payload,length);
        cbk->length = length - sizeof(*l2cap);
    } else {
        if(cbk->blk != NULL) {
            if(cbk->length + acl->length <= cbk->total) 
                length = acl->length;
            else
                length = cbk->total - cbk->length;
            memcpy(cbk->blk + cbk->length + sizeof(*l2cap),acl->payload,length);
            cbk->length += length;
        }
    }

    LOGD("cid %x,length %x",l2cap->cid,l2cap->length);

    if(cbk->length >= cbk->total) {
        l2cap = cbk->blk;
        cbk->blk = NULL;
        cbk->length = 0;
        l2cap_handler(l2cap);
    }
}
