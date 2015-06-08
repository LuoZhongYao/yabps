#ifndef __DRV_H__
#define __DRV_H__
#include <object.h>
#include <errno.h>
#include <transport.h>

__BEGIN_DECLS

int driver_send(transport_t channel,const void *msg,size_t mlen);
int driver_receiv(transport_t channel,void *msg,size_t nlen);


__END_DECLS
#endif
