#include <l2cap.h>
#include <assert.h>
#include <zl/log.h>

#ifdef __LOG_L2CAP__
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

static void l2cap_connection_response(u8 identifier,u16 dest_cid,u16 src_cid,u16 result,u16 status)
{
    l2cap_connection_response_t *res;
    //l2cap_signaling_t *sig;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&res,8);
    //res = (l2cap_connection_response_t*)sig->payload;
    res->length = 8;
    res->status = status;
    res->code = 0x03;
    res->identifier = identifier;
    res->dest_cid = dest_cid;
    res->src_cid = src_cid;
    res->result = result;
    l2cap_signaling_send((l2cap_signaling_t *)res,1,0x32,2);
}

static void l2cap_connection_request(u8 identifier,u16 src_cid,u16 psm)
{
    l2cap_connection_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,4);
    req->code = 0x02;
    req->psm = psm;
    req->length = 0x4;
    req->src_cid = src_cid;
    req->identifier = identifier;
    l2cap_signaling_send((l2cap_signaling_t *)req,1,0x32,2);
}

static void l2cap_configure_request(u8 identifier,u16 dest_cid,u16 cflag,u8 mtu_type,
        u8 mtu_length,u16 max_mtu)
{
    l2cap_configure_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,8);
    req->code = 0x04;
    req->identifier = identifier;
    req->length = 8;
    req->dest_cid = dest_cid;
    req->cflag = cflag;
    req->mtu_type = mtu_type;
    req->mtu_length = mtu_length;
    req->max_mtu = max_mtu;
    l2cap_signaling_send((l2cap_signaling_t *)req,1,0x32,2);
}

static void l2cap_configure_response(u8 identifier,u16 src_cid,u16 cflag,u16 result)
{
    l2cap_configure_response_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,6);
    req->code = 0x05;
    req->identifier = identifier;
    req->length = 6;
    req->src_cid = src_cid;
    req->result = 0x0;
    l2cap_signaling_send((l2cap_signaling_t *)req,1,0x32,2);
}

void l2cap_signaling_handler(l2cap_signaling_t *sig)
{
    SIG_LOGD("code %x,identifier %x,length %x",
            sig->code,sig->identifier,sig->length);
    switch(sig->code) {
    case 0x02:
        {
            l2cap_connection_request_t *req = (l2cap_connection_request_t *)sig;
            SIG_LOGD("psm %x,src_cid %x",req->psm,req->src_cid);
            //l2cap_connection_request(req->identifier,req->src_cid,req->psm);
            l2cap_connection_response(sig->identifier,req->src_cid,0x40,0,0);
           l2cap_configure_request(0x67,0x40,0,0x01,0x02,1024);
        }
        break;
    case 0x03:
        break;
    case 0x04:
        {
           l2cap_configure_response(0x67,0x40,0,0);
        }
        break;
    }
}
