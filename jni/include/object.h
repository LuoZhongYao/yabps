#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <zl/types.h>
__BEGIN_DECLS

#define NEW(__obj,...)       new_ ## __obj(__VA_ARGS__)
#define DELETE(__obj,...)    delete_ ## __obj(__VA_ARGS__)
#define METHON(__obj,__methon,__defualt,...)    ({\
        __typeof__((__obj)->__methon((__obj),##__VA_ARGS__)) _v =\
        (__defualt);\
        if((__obj) && (__obj)->__methon) { \
            _v = (__obj)->__methon((__obj),##__VA_ARGS__);\
        }_v;})


__END_DECLS
#endif
