#include <l2cap.h>
#include <assert.h>
#include <zl/log.h>

#ifdef __LOG_ACL__
#define SIG_LOGD(fmt,...)   LOGD("[SIG] "fmt,##__VA_ARGS__)
#else
#define SIG_LOGD(...)
#endif

void *alloc_l2cap_signaling_packed(l2cap_signaling_t **sig,u16 length)
{
    l2cap_t *l2cap;
    void *buffer;
    buffer = alloc_l2cap_packed(&l2cap,sizeof(l2cap_signaling_t) + length);
    assert(l2cap);
    *sig = (__typeof__(*sig))l2cap->payload;
    return buffer;
}

int l2cap_signaling_send(l2cap_signaling_t *sig,u16 cid,u16 handle,u8 flags)
{
    l2cap_t *l2cap = packet_of(l2cap_t,sig);
    l2cap->cid = cid;
    l2cap->length =  sizeof(*sig) + sig->length;
    return l2cap_send(l2cap,handle,flags);
}

static void l2cap_connection_response(u16 identifier,u16 dest_cid,u16 src_cid,u16 result,u16 status)
{
    l2cap_connection_response_t *res;
    l2cap_signaling_t *sig;
    alloc_l2cap_signaling_packed(&sig,8);
    res = (l2cap_connection_response_t*)sig->payload;
    res->status = status;
    res->code = 0x03;
    res->identifier = identifier + 1;
    res->length = 8;
    res->dest_cid = dest_cid;
    res->src_cid = src_cid;
    res->result = result;
    l2cap_signaling_send(sig,1,32,2);
}

void l2cap_signaling_handler(l2cap_signaling_t *sig)
{
    SIG_LOGD("code %x,identifier %x,length %x",
            sig->code,sig->identifier,sig->length);
    if(sig->code == 0x02) {
        l2cap_connection_request_t *req = (l2cap_connection_request_t *)sig->payload;
        SIG_LOGD("psm %x,src_cid %x",req->psm,req->src_cid);
        l2cap_connection_response(sig->identifier,req->src_cid,req->src_cid,0,0);
    }
}
