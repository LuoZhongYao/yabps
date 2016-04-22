#define TAG "ZL.L2CAP"

#include <l2cap.h>
#include <assert.h>
#include <byteorder.h>
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

static void l2cap_configure_response(u16 handle,
                                     u8 identifier,
                                     u16 remote_cid,
                                     u16 flags,
                                     u16 result,
                                     l2cap_config_option_t *opt,
                                     u16 length)
{
    l2cap_configure_response_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,6 + length);
    req->code       = 0x05;
    req->identifier = identifier;
    req->length     = cpu_to_le16(6 + length);
    req->remote_cid = cpu_to_le16(remote_cid);
    req->cflag      = cpu_to_le16(flags);
    req->result     = cpu_to_le16(result);
    memcpy(req->option,opt,length);
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}

static void l2cap_conn_response(u16 handle,l2cap_cbk_t *cbk,u8 identifier,u16 result,u16 status)
{
    l2cap_connection_response_t *res;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&res,8);
    res->length     = cpu_to_le16(8);
    res->status     = cpu_to_le16(status);
    res->local_cid  = cpu_to_le16(cbk->local_cid);
    res->remote_cid = cpu_to_le16(cbk->remote_cid);
    res->result     = cpu_to_le16(result);
    res->code       = L2CAP_CONN_RSP;
    res->identifier = identifier;
    l2cap_signaling_send(handle,(l2cap_signaling_t *)res);
}

static void l2cap_information_response(u16 handle,u8 identifier,u16 info_type,u16 result,u16 size,void *data)
{
    l2cap_information_response_t *res;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&res,size + 4);
    res->identifier = identifier;
    res->code       = L2CAP_INFO_RSP;
    res->length     = cpu_to_le16(size + 4);
    res->info_type  = cpu_to_le16(info_type);
    res->result     = cpu_to_le16(result);
    memcpy(res->data,data,size);
    l2cap_signaling_send(handle,(l2cap_signaling_t*)res);
}

static void l2cap_connection_request(u16 handle,u16 remote_cid,u16 psm)
{
    l2cap_connection_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,4);
    req->code        = 0x02;
    req->length      = cpu_to_le16(4);
    req->psm         = cpu_to_le16(psm);
    req->remote_cid  = cpu_to_le16(remote_cid);
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}

static void l2cap_configure_request(u16 handle,
                                    u16 local_cid,
                                    u16 cflag,
                                    l2cap_config_option_t *opt,
                                    u16 length)
{
    l2cap_configure_request_t *req;
    alloc_l2cap_signaling_packed((l2cap_signaling_t **)&req,4 + length);
    req->code        = 0x04;
    req->length      = cpu_to_le16(4 + length);
    req->local_cid   = cpu_to_le16(local_cid);
    req->cflag       = cpu_to_le16(cflag);
    memcpy(req->option,opt,length);
    l2cap_signaling_send(handle,(l2cap_signaling_t *)req);
}


static void l2cap_handle_conn_req(u16 handle,l2cap_connection_request_t *req)
{
    req->psm  = le16_to_cpu(req->psm);
    req->remote_cid = le16_to_cpu(req->remote_cid);

    struct l2cap_protocol *protocol = find_protocol_psm(req->psm);
    if(protocol == NULL) {
        LOGD("psm %x,remote_cid %x",req->psm,req->remote_cid);
    } else {
        l2cap_cbk_t *cbk = l2cap_alloc_cbk(req->remote_cid,L2CAP_CONFIG,protocol->task);
        l2cap_config_option_t *opt = malloc(sizeof(l2cap_config_option_t) + 2);
        opt->length = 2;
        opt->type = L2CAP_CFG_MTU;
        *(u16*)opt->data = cpu_to_le16(672);
        l2cap_conn_response(handle,cbk,req->identifier,L2CAP_CONN_SUCCESS,0);
        l2cap_configure_request(handle,req->remote_cid,0,opt,4);
        free(opt);
    }
}

static void l2cap_handle_information_req(u16 handle,l2cap_information_request_t *req)
{
    req->info_type = le16_to_cpu(req->info_type);
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

static void l2cap_handle_configure_req(u16 handle,l2cap_configure_request_t *req)
{
    req->local_cid  = le16_to_cpu(req->local_cid);
    req->cflag = le16_to_cpu(req->cflag);

    l2cap_cbk_t *l2c = find_l2cap_cbk(req->local_cid);
    l2cap_config_option_t *opt = req->option;
    const void *opt_end = (void*)opt + req->length;

    while((void*)opt < opt_end) {
        switch(opt->type & 0x7F) {
        case L2CAP_CFG_MTU: {
            LOGD("MTU : %d\n",le16_to_cpu(((u16 *)opt->data)[0]));
            l2cap_configure_response (
                                      handle,
                                      req->identifier,
                                      l2c->remote_cid,
                                      req->cflag,
                                      L2CAP_CFG_SUCCESS,
                                      opt,
                                      2 + L2CAP_CFG_MTU_LEN);
        } break;

        case L2CAP_CFG_FLUSHTO: {
            LOGD("FLUSH TIMEOUT : %d\n",le16_to_cpu(((u16 *)opt->data)[0]));
        } break;

        case L2CAP_CFG_QOS: {
            LOGD("QOS\n");
        } break;

        case L2CAP_CFG_FCR: {
            LOGD("FCR\n");
        } break;

        case L2CAP_CFG_FCS: {
            LOGD("FCS\n");
        } break;

        case L2CAP_CFG_EXT_FLOW: {
            LOGD("EXT_FLOW\n");
        } break; 

        case L2CAP_CFG_EXT_WIN_SIZE: {
            LOGD("WIN_SIZE\n");
        } break;
        }
        opt = (void*)opt + opt->length + L2CAP_CFG_OVERHEAD; 
    }
}

void l2cap_signaling_handler(u16 handle,l2cap_signaling_t *sig)
{
    sig->length = le16_to_cpu(sig->length);

#define SIG_INST(t) t * msg =  (t *)sig
    switch(sig->code) {
    case L2CAP_CONN_REQ: {
        SIG_INST(l2cap_connection_request_t);
        LOGD("L2CAP_CONN_REQ : psm %x,remote_cid %x\n",msg->psm,msg->remote_cid);
        l2cap_handle_conn_req(handle,msg);
    } break;

    case L2CAP_CONN_RSP: {
        SIG_INST(l2cap_connection_response_t);
        LOGD("L2CAP_CONN_RSP : local_cid %x,remote_cid %x,result %x,status %x\n",
             msg->local_cid,msg->remote_cid,msg->result,msg->status);
    } break;

    case L2CAP_CFG_REQ: {
        SIG_INST(l2cap_configure_request_t);
        LOGD("L2CAP_CFG_REQ : local_cid %x,cflags %x\n", msg->local_cid,msg->cflag);
        l2cap_handle_configure_req(handle,msg);
    } break;

    case L2CAP_CFG_RSP: {
        LOGD("L2CAP_CFG_RSP\n");
    } break;

    case L2CAP_INFO_REQ: {
        SIG_INST(l2cap_information_request_t);
        LOGD("L2CAP_INFO_REQ info_type %x\n",msg->info_type);
        l2cap_handle_information_req(handle,msg);
    } break;

    case L2CAP_DISCONN_REQ: {
        LOGD("L2CAP_DISCONN_REQ\n");
    } break;

    default:
        LOGE("SIG ? %x,identifier %x,%x\n",
             sig->code,sig->identifier,sig->length);
        break;
    }
#undef SIG_INST
}
