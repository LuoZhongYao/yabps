#include <l2cap.h>
#include <assert.h>
#include <zl/log.h>
#include "l2cap_layer.h"

#ifdef __LOG_L2CAP__
#define SIG_LOGD(fmt,...)   LOGD("[SIG] "fmt,##__VA_ARGS__)
#else
#define SIG_LOGD(...)
#endif

/*************** Signaling Identifier *****************/
static u8 sig_id(void)
{
    static u8 id = 0;
    id++;
    if(id == 0)
        id = 1;
    return id;
}

void *alloc_l2cap_signaling_packed(l2cap_signaling_t **sig,u16 length)
{
    l2cap_t *l2cap;
    void *buffer;
    buffer = alloc_l2cap_packed(&l2cap,sizeof(l2cap_signaling_t) + length);
    assert(l2cap);
    l2cap->cid = L2CAP_SIG_CID;
    l2cap->length = length + sizeof(l2cap_signaling_t);
    *sig = (__typeof__(*sig))l2cap->payload;
    (*sig)->identifier = sig_id();
    return buffer;
}

int l2cap_signaling_send(l2cap_signaling_t *sig,u16 handle)
{
    l2cap_t *l2cap = packet_of(l2cap_t,sig);
    return l2cap_send(l2cap,handle);
}

static void l2cap_connection_request(u16 scid,u16 psm)
{
    l2cap_connection_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,4);
    req->code = 0x02;
    req->psm = psm;
    req->length = 0x4;
    req->scid = scid;
    l2cap_signaling_send((l2cap_signaling_t *)req,0x32);
}

static void l2cap_configure_request(u16 dcid,u16 cflag,u8 mtu_type,
        u8 mtu_length,u16 max_mtu)
{
    l2cap_configure_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,8);
    req->code = 0x04;
    req->length = 8;
    req->dcid = dcid;
    req->cflag = cflag;
    req->mtu_type = mtu_type;
    req->mtu_length = mtu_length;
    req->max_mtu = max_mtu;
    l2cap_signaling_send((l2cap_signaling_t *)req,0x32);
}

static void l2cap_configure_response(u8 identifier,u16 scid,u16 result)
{
    l2cap_configure_response_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,6);
    req->code = 0x05;
    req->identifier = identifier;
    req->length = 6;
    req->scid = scid;
    req->result = result;
    l2cap_signaling_send((l2cap_signaling_t *)req,0x32);
}

static void l2cap_conn_response(l2cap_cbk_t *cbk,u8 identifier,u16 result,u16 status)
{
    l2cap_connection_response_t *res;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&res,8);
    res->length = 8;
    res->status = status;
    res->code = L2CAP_CONN_RSP;
    res->identifier = identifier;
    res->dcid = cbk->dcid;
    res->scid = cbk->scid;
    res->result = result;
    l2cap_signaling_send((l2cap_signaling_t *)res,0x32);
}

static void l2cap_conn_req(l2cap_connection_request_t *req)
{
    l2cap_listen_t *listen = find_l2cap_listen(req->psm);
    if(listen == NULL) {
        SIG_LOGD("psm %x,scid %x",req->psm,req->scid);
        //l2cap_conn_response(req->identifier,req->scid,
        //        0x40,L2CAP_CONN_REF_RES,0);
    } else {
        l2cap_cbk_t *cbk = l2cap_alloc_cbk(listen,req->scid,L2CAP_CONFIG);
        l2cap_conn_response(cbk,req->identifier,L2CAP_CONN_SUCCESS,0);
        l2cap_configure_request(req->scid,0,1,2,1024);
    }
}

void l2cap_signaling_handler(l2cap_signaling_t *sig)
{
    SIG_LOGD("code %x,identifier %x,length %x",
            sig->code,sig->identifier,sig->length);
    switch(sig->code) {
    case L2CAP_CONN_REQ:
        {
            l2cap_connection_request_t *req = (l2cap_connection_request_t *)sig;
            l2cap_conn_req(req);
        }
        break;
    case L2CAP_CONN_RSP:
        break;
    case L2CAP_CFG_REQ:
        {
            //l2cap_configure_request_t *req = (l2cap_configure_request_t *)sig;
            //l2cap_configure_response(req->identifier,req->dcid,0);
        }
        break;
    }
}
