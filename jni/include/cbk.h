/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jul 02
 ************************************************/
#ifndef __CBK_H__
#define __CBK_H__
#include <zl/types.h>
#include <stdbool.h>
#include <list.h>
#include <bd_addr.h>
__BEGIN_DECLS

typedef struct {
    struct list_head list;
    u8  private_data[0];
} cbk_t;

cbk_t *cbk_alloc(struct list_head *cbks,const size_t length);
cbk_t *find_cbk(struct list_head *cbks,const void *who,
        bool (*cmp)(const void *,const void *));
void cbk_free(cbk_t *cbk);


__END_DECLS
#endif

