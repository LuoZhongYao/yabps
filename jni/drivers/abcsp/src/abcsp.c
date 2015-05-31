#include <drv.h>
#include <zl/util.h>
#include <assert.h>
#include <abcsp.h>

static int __send(drv_t *drv,int ch,const void *buffer,size_t len)
{
    MessageStructure *msg = __new(MessageStructure);
    msg->buf = (void*)buffer;
    msg->buflen = len;
    return abcsp_sendmsg(msg,ch,1);
}

static int __receiv(drv_t *drv,int ch __unused,void *buffer,size_t len)
{
    return abcsp_uart_deliverbytes(buffer,len);
}

drv_t *new_abcsp(drv_t *drv)
{

    drv_t *abcsp = __new(drv_t);
    assert(abcsp != NULL);
    assert(drv != NULL);
    abcsp->send = __send;
    drv->receiv = __receiv;
    abcsp_init();
    return abcsp;
}

drv_t *delete_abcsp(drv_t *abcsp)
{
    drv_t *drv;
    assert(abcsp != NULL);
    drv = abcsp->top;
    delete(abcsp);
    return drv;
}


