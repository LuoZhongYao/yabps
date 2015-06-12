#include <acl.h>
#include <l2cap.h>
#include <hci.h>
#include <zl/log.h>
#include <zl/util.h>
#include <assert.h>

void acl_handler(acl_t *acl)
{
    blk_t *blk;
    l2cap_t *l2cap;
    u16 length;
    l2cap = (void*)acl->payload;
    blk = get_blk(acl->handle,hci_blks,MAX_ACL_CONNECTIONS);
    if(blk == NULL) {
#ifdef __LOG_ACL__
        LOGE("Can't found acl counnection");
#endif
        return;
    }

    if(acl->flags == 0x2) {
        __test_delete(blk->blk);
        blk->total = l2cap->length;
        blk->blk = malloc(sizeof(*l2cap) + blk->total);
        assert(blk->blk);
        if(blk->total <= blk->length + sizeof(*l2cap))
            length = acl->length;
        else
            length = blk->length + sizeof(*l2cap);
        memcpy(blk->blk,l2cap->paylod,length);
        blk->handle = acl->handle;
    } else {
        if(blk->blk != NULL) {
            if(blk->length + acl->length <= blk->total) 
                length = acl->length;
            else
                length = blk->total - blk->length;
            memcpy(blk->blk + blk->length + sizeof(*l2cap),l2cap->paylod,length);
            blk->length += length;
        }
    }
    if(blk->length >= blk->total) {
        l2cap = blk->blk;
        blk->blk = NULL;
        blk->length = 0;
        l2cap_handler(l2cap);
    }
}
