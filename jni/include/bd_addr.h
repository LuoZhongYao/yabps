/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 10
 ************************************************/
#ifndef __BD_ADDR_H__
#define __BD_ADDR_H__
#include <zl/types.h>
__BEGIN_DECLS
#include <stdbool.h>

typedef struct {
    u32 lap:24;
    u32 uap:8;
    u16 nap;
}__packed bd_addr_t;

static inline bool cmp_bdaddr(const bd_addr_t *a,const bd_addr_t *b)
{
    return (a->nap == b->nap) && (a->uap == b->uap) && (a->lap == b->lap);
}

__END_DECLS
#endif

