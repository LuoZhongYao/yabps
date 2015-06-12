/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __ACL_H__
#define __ACL_H__

#include <zl/types.h>

__BEGIN_DECLS

#define MAX_ACL_CONNECTIONS     7

typedef struct {
    u16 handle:12;
    u16 flags:4;
    u16 length;
    u8 payload[0];
}__packed acl_t;


__END_DECLS
#endif

