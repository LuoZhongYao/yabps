#include <drv.h>
#include <zl/util.h>
#include <assert.h>
#include <abcsp.h>
#include <zl/log.h>

static int __send(drv_t *drv,transport_channel_t ch,const void *buffer,size_t len)
{
    MessageStructure *msg = __new(MessageStructure);
    LOGD("abcsp send channel %d data.",ch);
    msg->buf = (void*)buffer;
    msg->buflen = len;
    return abcsp_sendmsg(msg,ch,1);
}

static int __receiv(drv_t *drv,transport_channel_t ch __unused,void *buffer,size_t len)
{
    return abcsp_uart_deliverbytes(buffer,len);
}

static int __start(drv_t *drv)
{
    abcsp_init();
    return 0;
}

drv_t *new_abcsp(void)
{

    drv_t *abcsp = __new(drv_t);
    assert(abcsp != NULL);
    abcsp->send   = __send;
    abcsp->receiv = __receiv;
    abcsp->start  = __start;
    return abcsp;
}

void delete_abcsp(drv_t *abcsp)
{
    assert(abcsp != NULL);
    delete(abcsp);
}
