/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __ACL_H__
#define __ACL_H__

#include <zl/types.h>
#include <transport.h>
#include <zl/util.h>

__BEGIN_DECLS

#define MAX_ACL_LINKS     7

typedef struct {
    u16 handle:12;
    u16 flags:4;
    u16 length;
    u8 payload[0];
}__packed acl_t;

acl_t *alloc_acl_packed(u16 length);
#define packet_of(type,ptr) ({\
    const void *__mptr = (ptr);\
    (type *)((u8 *)__mptr - offsetof(type,payload));})

__END_DECLS
#endif

