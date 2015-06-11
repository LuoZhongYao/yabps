/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 10
 ************************************************/
#ifndef __BD_ADDR_H__
#define __BD_ADDR_H__
#include <zl/types.h>
__BEGIN_DECLS

typedef struct {
    u32 lap:24;
    u32 uap:8;
    u16 nap;
}__packed bd_addr_t;

__END_DECLS
#endif

