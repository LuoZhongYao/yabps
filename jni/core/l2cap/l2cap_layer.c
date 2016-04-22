#include "l2cap_layer.h"
#include <assert.h>
#include <cbk.h>
static LIST_HEAD(l2c_protocol);
static LIST_HEAD(l2c_cbk);

static u16 l2cap_cid_alloc(void)
{
    static u16 cid = 0x40;
    for(u16 id = cid + 1;id != cid;id++){
        if(!find_l2cap_cbk(id)) {
            cid = id;
            return id;
        }
        if(id >= L2CAP_MAX_CID) {
            id = L2CAP_MIN_CID - 1;
        }
    }

    return 0;
}

static bool protocol_cmp_psm(const void *pos,const void *who)
{
    struct l2cap_protocol *protocol = (struct l2cap_protocol *)pos;
    if(protocol->psm == *(const u16 *)who)
        return true;
    return false;
}

struct l2cap_protocol *find_protocol_psm(u16 psm)
{
    cbk_t *cbk = find_cbk(&l2c_protocol,&psm,protocol_cmp_psm);
    if(cbk != NULL)
        return (struct l2cap_protocol*)cbk->private_data;
    return NULL;
}

void l2cap_register_protocol(u16 psm,l2cap_task task)
{
    struct l2cap_protocol *protocol = find_protocol_psm(psm);
    if(protocol == NULL) {
        cbk_t *cbk = cbk_alloc(&l2c_protocol,sizeof(*protocol));
        assert(cbk != NULL);
        protocol = (struct l2cap_protocol *)cbk->private_data;
    }
    protocol->task = task;
    protocol->psm = psm;
}

l2cap_cbk_t *l2cap_alloc_cbk(
                             u16 remote_cid,
                             enum l2cap_state state,
                             l2cap_task task
                             )
{
    l2cap_cbk_t *l2cap;
    cbk_t *cbk = cbk_alloc(&l2c_cbk,sizeof(*l2cap));
    assert(cbk);
    l2cap             = (l2cap_cbk_t*)cbk->private_data;
    l2cap->state      = state;
    l2cap->local_cid  = l2cap_cid_alloc();
    l2cap->remote_cid = remote_cid;
    l2cap->task       = task;
    return l2cap;
}

void l2cap_cbk_free(l2cap_cbk_t *cbk)
{
    cbk_free(cbk_of(cbk));
}

static bool cmp_cid(const void *pos,const void *who)
{
    l2cap_cbk_t *l2cap = (l2cap_cbk_t *)pos;
    if(l2cap->local_cid == *(const u16 *)who)
        return true;
    return false;
}

l2cap_cbk_t *find_l2cap_cbk(const u16 local_cid)
{
    cbk_t *cbk = find_cbk(&l2c_cbk,&local_cid,cmp_cid);
    if(cbk)
        return (l2cap_cbk_t *)cbk->private_data;
    return NULL;
}

