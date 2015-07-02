
#ifndef __ZL_UTIL_H__
#define __ZL_UTIL_H__
#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef __cplusplus
#define delete(x) free(x)
#endif

__BEGIN_DECLS

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
# define __same_type(a, b) __builtin_types_compatible_p(__typeof__(a), __typeof__(b))
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#define container_of(ptr, type, member) ({                      \
        const __typeof__( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define __new(t)        (t *)calloc(1,sizeof(t))
#define __test_delete(x) do{if((x)) {delete((x));}(x) = NULL;}while(0)

__END_DECLS
#endif
