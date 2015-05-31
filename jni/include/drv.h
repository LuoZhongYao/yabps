#ifndef __DRV_H__
#define __DRV_H__
#include <object.h>
#include <errno.h>
__BEGIN_DECLS

typedef struct drv_t drv_t;

struct drv_t {
    int (*start)(drv_t *__z);
    int (*configure)();
    int (*receiv)(drv_t *__z,int ch,void *__buffer,size_t __n);
    int (*send)(drv_t *__z,int ch,const void *__buffer,size_t __n);
    int (*close)(drv_t *__z);
    drv_t *top;
};

#define DRV_RECEIV  0
#define drv_start(__drv)                \
    METHON(__drv,start,0)
#define drv_configure(__drv,...)        \
    METHON(__drv,configure,0,##__VA_ARGS__)
#define drv_send(__drv,ch,__buffer,__n)   \
    METHON(__drv,send,-1,ch,__buffer,__n)
#define drv_receiv(__drv,__buffer,__n)   \
    METHON(__drv,receiv,-1,0,__buffer,__n)
#define drv_close(__drv)                \
    METHON(__drv,0,close)

__END_DECLS
#endif
