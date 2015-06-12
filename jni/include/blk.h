/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __BLK_H__
#define __BLK_H__

#include <zl/types.h>
#include <zl/util.h>
#include <stdbool.h>
#include <assert.h>
__BEGIN_DECLS

typedef struct {
    u16 total;
    u16 length;
    u16 handle;
    bool used;
    void *blk;
} blk_t;

static inline blk_t *get_blk(u16 handle,blk_t* blks,u16 nr)
{
    for(int i = 0;i < nr;i++) {
        if(blks[i].used && blks[i].handle == handle)
            return blks + i;
    }
    return NULL;
}

static inline blk_t *alloc_blk(blk_t *blks,u16 nr)
{
    for(int i = 0;i < nr;i++){
        if(!blks[i].used) {
            blks[i].used = true;
            blks[i].total = 0;
            blks[i].length = 0;
            blks[i].handle = 0;
            blks[i].blk = NULL;
            return blks + i;
        }
    }
    return NULL;
}

static inline void free_blk(blk_t *blk)
{
    assert(blk);
    if(blk->blk)
        delete(blk->blk);
    blk->used = false;
}

__BEGIN_DECLS
#endif

