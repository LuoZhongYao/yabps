#include "l2cap_layer.h"
#include <assert.h>
#include <cbk.h>
static LIST_HEAD(l2c_listens);
static LIST_HEAD(l2c_cbk);

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
