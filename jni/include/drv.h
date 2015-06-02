#ifndef __DRV_H__
#define __DRV_H__
#include <object.h>
#include <errno.h>
#include <transport.h>
__BEGIN_DECLS

typedef struct drv_t drv_t;

struct drv_t {
    int (*start)(drv_t *__z);
    int (*stop)(drv_t *__z);
    int (*close)(drv_t *__z);
    int (*configure)();
    int (*receiv)(drv_t *__z,transport_channel_t ch,void *__buffer,size_t __n);
    int (*send)(drv_t *__z,transport_channel_t ch,const void *__buffer,size_t __n);
    int (*server)(drv_t *__z,drv_t *server);
    int (*client)(drv_t *__z,drv_t *client);
    int (*external)(drv_t *__z,int type,...);
};

#define DRV_RECEIV  0
#define drv_start(__drv)                \
    METHON(__drv,start,0)
#define drv_configure(__drv,...)        \
    METHON(__drv,configure,0,##__VA_ARGS__)
#define drv_send(__drv,__ch,__buffer,__n)   \
    METHON(__drv,send,-1,__ch,__buffer,__n)
#define drv_receiv(__drv,__ch,__buffer,__n)   \
    METHON(__drv,receiv,-1,__ch,__buffer,__n)
#define drv_close(__drv)                \
    METHON(__drv,0,close)
#define drv_external(__drv,type,...)\
    METHON(__drv,external,-ENOSYS,type,##__VA_ARGS__)
#define drv_client(__drv,__client)  \
    METHON(__drv,client,-ENOENT,__client)
#define drv_server(__drv,__server) \
    METHON(__drv,server,-ENOENT,__server)

__END_DECLS
#endif
