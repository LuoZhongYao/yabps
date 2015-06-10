#include <assert.h>
#include <zl/util.h>
#include <zl/log.h>
#include <transport.h>
#include <drv.h>
#include <hci.h>

typedef void (*ch_handler_t)(void *data);
extern void hci_event_handler();

static ch_handler_t channels [] = {
    [CHANNEL_HCI] = hci_event_handler,
};

int hci_receiv(transport_t channel,MessageStructure *src)
{
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
            channel <= CHANNEL_VM &&
            channels[channel])
        channels[channel](src->buf);
    else
        LOGE("The channel(%d) can not be routed",channel);
    return 0;
}

int hci_send(transport_t channel,const void *buffer,size_t mlen)
{
    MessageStructure *msg = __new(MessageStructure);
    msg->buf = (void*)buffer;
    msg->buflen = mlen;
    return abcsp_sendmsg((void*)msg,channel,1);
}
