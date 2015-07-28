#include "l2cap_layer.h"
#include <assert.h>
#include <cbk.h>
static LIST_HEAD(l2c_listens);
static LIST_HEAD(l2c_cbk);

static u16 l2cap_cid_alloc(void)
{
    static u16 cid = 1024;
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

void register_l2cap_listen(const u16 psm,
        int (*handler)(l2cap_cbk_t *cbk,void *args))
{
    l2cap_listen_t *l2cap_listen; 
    cbk_t *cbk = cbk_alloc(&l2c_listens,sizeof(*l2cap_listen));
    assert(cbk);
    l2cap_listen = (l2cap_listen_t*)cbk->private_data;
    l2cap_listen->psm = psm;
    l2cap_listen->handler = handler;
}

void deregister_l2cap_listen(l2cap_listen_t *listen)
{
    cbk_free(cbk_of(listen));
}

l2cap_cbk_t *l2cap_alloc_cbk(l2cap_listen_t *listen,u16 cid,
        enum l2cap_state state)
{
    l2cap_cbk_t *l2cap;
    cbk_t *cbk = cbk_alloc(&l2c_cbk,sizeof(*l2cap));
    assert(l2cap);
    l2cap = (l2cap_cbk_t*)cbk->private_data;
    l2cap->state = state;
    l2cap->dcid  = cid;
    l2cap->scid  = l2cap_cid_alloc();
    l2cap->psm   = listen->psm;
    l2cap->args  = listen->args;
    l2cap->handler = listen->handler;
    return l2cap;
}

void l2cap_cbk_free(l2cap_cbk_t *cbk)
{
    cbk_free(cbk_of(cbk));
}

static bool cmp_psm(const void *pos,const void *who)
{
    l2cap_listen_t *l2cap = (l2cap_listen_t *)pos;
    if(l2cap->psm == *(const u16 *)who)
        return true;
    return false;
}

l2cap_listen_t *find_l2cap_listen(const u16 psm)
{
    cbk_t *cbk = find_cbk(&l2c_listens,&psm,cmp_psm);
    if(cbk)
        return (l2cap_listen_t*)cbk->private_data;
    return NULL;
}

static bool cmp_cid(const void *pos,const void *who)
{
    l2cap_cbk_t *l2cap = (l2cap_cbk_t *)pos;
    if(l2cap->cid == *(const u16 *)who)
        return true;
    return false;
}

l2cap_cbk_t *find_l2cap_cbk(const u16 cid)
{
    cbk_t *cbk = find_cbk(&l2c_cbk,&cid,cmp_cid);
    if(cbk)
        return (l2cap_cbk_t *)cbk->private_data;
    return NULL;
}

