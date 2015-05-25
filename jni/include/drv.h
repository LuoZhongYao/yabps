#ifndef __DRV_H__
#define __DRV_H__
#include <sys/types.h>
__BEGIN_DECLS

typedef struct drv_t drv_t;

struct drv_t {
    int (*start)(drv_t *__z);
    int (*configure)();
    int (*read)(drv_t *__z,void *__buffer,size_t __n);
    int (*write)(drv_t *__z,const void *__buffer,size_t __n);
    int (*close)(drv_t *__z);
};

drv_t *new_drv(const char *path);
void delete_drv(drv_t *__z);

#define drv_apply(__drv,__methon,__default,...)  ({\
        __typeof__(__drv->__methon(__drv,##__VA_ARGS__)) _v = __default;\
        if(__drv && __drv->__methon) { \
            _v = __drv->__methon(__drv,##__VA_ARGS__);\
        }_v;})

#define drv_start(__drv)                drv_apply(__drv,start,0)
#define drv_configure(__drv,...)        drv_apply(__drv,configure,0,##__VA_ARGS__)
#define drv_write(__drv,__buffer,__n)   drv_apply(__drv,write,-1,__buffer,__n)
#define drv_read(__drv,__buffer,__n)    drv_apply(__drv,read,-1,__buffer,__n)
#define drv_close(__drv)                drv_apply(__drv,0,close)

__END_DECLS
#endif
