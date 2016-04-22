#define TAG "ZL.L2CAP"

#include <l2cap.h>
#include <assert.h>
#include <zl/log.h>
#include "l2cap_layer.h"

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

int l2cap_signaling_send(u16 handle,l2cap_signaling_t *sig)
{
    l2cap_t *l2cap = packet_of(l2cap_t,sig);
    return l2cap_send(handle,l2cap);
}

static void l2cap_configure_response(u16 handle,u8 identifier,u16 scid,u16 result)
{
    l2cap_configure_response_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,6);
    req->code = 0x05;
    req->identifier = identifier;
    req->length = 6;
    req->scid = scid;
    req->result = result;
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}

static void l2cap_conn_response(u16 handle,l2cap_cbk_t *cbk,u8 identifier,u16 result,u16 status)
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
    l2cap_signaling_send(handle,(l2cap_signaling_t *)res);
}

static void l2cap_information_response(u16 handle,u8 identifier,u16 info_type,u16 result,u16 size,void *data)
{
    l2cap_information_response_t *res;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&res,size + 4);
    res->length = size + 4;
    res->info_type = info_type;
    res->identifier = identifier;
    res->code = L2CAP_INFO_RSP;
    res->result =result;
    memcpy(res->data,data,size);
    l2cap_signaling_send(handle,(l2cap_signaling_t*)res);
}

static void l2cap_connection_request(u16 handle,u16 scid,u16 psm)
{
    l2cap_connection_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,4);
    req->code = 0x02;
    req->psm = psm;
    req->length = 0x4;
    req->scid = scid;
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}

static void l2cap_configure_request(u16 handle,u16 dcid,u16 cflag,u8 mtu_type,
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
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}


static void l2cap_handle_conn_req(u16 handle,l2cap_connection_request_t *req)
{
    l2cap_listen_t *listen = find_l2cap_listen(req->psm);
    if(listen == NULL) {
        LOGD("psm %x,scid %x",req->psm,req->scid);
    } else {
        l2cap_cbk_t *cbk = l2cap_alloc_cbk(listen,req->scid,L2CAP_CONFIG);
        l2cap_conn_response(handle,cbk,req->identifier,L2CAP_CONN_SUCCESS,0);
        /*l2cap_configure_request(handle,req->scid,0,1,2,1024);*/
    }
}

static void l2cap_handle_information_req(u16 handle,l2cap_information_request_t *req)
{
    switch(req->info_type) {
    case 1:  {/* connectionless MTU */
        u16 mtu = 1024;
        l2cap_information_response(handle,req->identifier,req->info_type,0,sizeof(mtu),&mtu);
    } break;
    case 2: { /* Extended feature mask */
        u32 features = 0x280;
        l2cap_information_response(handle,req->identifier,req->info_type,0,sizeof(features),&features);
    } break;
    case 3: {/* fixed channels supported */
        u8 map[8] = {1,0};
        l2cap_information_response(handle,req->identifier,req->info_type,0,sizeof(map),&map);
    } break;
    }
}

void l2cap_signaling_handler(u16 handle,l2cap_signaling_t *sig)
{
#define SIG_INST(t) t * msg =  (t *)sig
    switch(sig->code) {
    case L2CAP_CONN_REQ: {
        SIG_INST(l2cap_connection_request_t);
        LOGD("L2CAP_CONN_REQ : psm %x,scid %x\n",msg->psm,msg->scid);
        l2cap_handle_conn_req(handle,msg);
    } break;

    case L2CAP_CONN_RSP: {
        SIG_INST(l2cap_connection_response_t);
        LOGD("L2CAP_CONN_RSP : dcid %x,scid %x,result %x,status %x\n",
             msg->dcid,msg->scid,msg->result,msg->status);
    } break;

    case L2CAP_CFG_REQ: {
        SIG_INST(l2cap_configure_request_t);
        LOGD("L2CAP_CFG_REQ : dcid %x,cflags %x,mtu_type %x,mut_length %x,max_mut %x\n",
             msg->dcid,msg->cflag,msg->mtu_type,msg->mtu_length,msg->max_mtu);
    } break;

    case L2CAP_INFO_REQ: {
        SIG_INST(l2cap_information_request_t);
        LOGD("L2CAP_INFO_REQ info_type %x\n",msg->info_type);
        l2cap_handle_information_req(handle,msg);
    } break;

    default:
        LOGE("L2CAP ? %x,identifier %x,%x\n",
             sig->code,sig->identifier,sig->length);
        break;
    }
#undef SIG_INST
}
