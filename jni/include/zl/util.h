#ifndef __ZL_UTIL_H__
#define __ZL_UTIL_H__
#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
__BEGIN_DECLS

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define __new(t)  (t *)calloc(1,sizeof(t))
__END_DECLS

#ifndef __cplusplus
#define delete(x) free(x)
#endif

#endif
