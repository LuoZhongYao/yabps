#include <cbk.h>
#include <zl/log.h>
#include <zl/util.h>
#include <assert.h>


#ifdef  __LOG_CBK__
#define CBK_LOGD(fmt,...)   LOGD("[CBK] "fmt,##__VA_ARGS__)
#else
#define CBK_LOGD(...)
#endif

cbk_t *cbk_alloc(struct list_head *cbks,const size_t length)
{
    cbk_t *cbk = malloc(sizeof(*cbk) + length);
    assert(cbk);
    list_add(&cbk->list,cbks);
    return cbk;
}

void cbk_free(cbk_t *cbk)
{
    assert(cbk);
    list_del(&cbk->list);
    free(cbk);
}

cbk_t *find_cbk(struct list_head *cbks,const void *who,
        bool (*cmp)(const void *pos,const void *who))
{
    cbk_t *cbk;
    assert(cbks);
    list_for_each(pos,cbks) {
        cbk = list_entry(pos,cbk_t,list);
        if(cmp(cbk->private_data,who))
            return cbk;
    }
    return NULL;
}
