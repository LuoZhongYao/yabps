#include <assert.h>
#include <zl/util.h>
#include <zl/log.h>
#include <transport.h>
#include <drv.h>
#include <hci.h>

#ifdef __DUMP_HCI__
#include <util/dump.h>
static dump_t *dump = NULL;
void hci_dump(dump_t *,transport_t,const void*,size_t);
#define __DUMP(ch,buff,len) ({if(dump == NULL) dump = new_dump("/data/user/hci.hex");hci_dump(dump,(ch),(buff),(len));})
#else
#define __DUMP(...)
#endif


typedef void (*ch_handler_t)(void *data);
extern void hci_event_handler();
extern void l2cap_handler();
extern void acl_handler();

static const ch_handler_t channels [] = {
    [CHANNEL_HCI] = hci_event_handler,
    [CHANNEL_ACL] = acl_handler,
};

int hci_receiv(transport_t channel,MessageStructure *src)
{
    __DUMP(channel,src->buf,src->buflen);
#ifdef __LOG_HCI_CHANNEL__
    switch(channel){
    case CHANNEL_ACL:
        LOGD("ACL");
        break;
    case CHANNEL_SCO:
        LOGD("SCO");
        break;
    case CHANNEL_HCI:
        LOGD("HCI");
        break;
    case CHANNEL_BCCMD:
        LOGD("BCCMD");
        break;
    case CHANNEL_VM:
        LOGD("VM");
        break;
    case CHANNEL_HQ:
        LOGD("HQ");
        break;
    default:
        break;
    }
#endif
    if(channel >= CHANNEL_UNRELIABLE &&
            channel < ARRAY_SIZE(channels) &&
            channels[channel]) {
        channels[channel](src->buf);
    } else {
        LOGE("The channel(%d) can not be routed",channel);
    }
    return 0;
}

int hci_send(transport_t channel,const void *buffer,size_t mlen)
{
    MessageStructure *msg = __new(MessageStructure);
    __DUMP((channel == CHANNEL_HCI ? 0x01 : channel),buffer,mlen);
    msg->buf = (void*)buffer;
    msg->buflen = mlen;
    return abcsp_sendmsg((void*)msg,channel,1);
}
