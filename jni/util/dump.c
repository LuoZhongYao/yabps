#include <util/dump.h>
#include <errno.h>
#include <string.h>
#include <zl/log.h>
dump_t *new_dump(const char *path __unused)
{
#ifdef __LOG_DUMP__
    dump_t *dump;
    dump = fopen(path,"w+");
    if(dump == NULL)
        LOGE("[DMP] %s",strerror(errno));
    return dump;
#else
    return NULL;
#endif
}

void dump_write(dump_t *dump __unused,
        const void *buffer __unused,size_t len __unused)
{
#ifdef __LOG_DUMP__
    if(dump)
        fwrite(buffer,1,len,dump);
#endif
}

void del_dump(dump_t *dump __unused)
{
#ifdef __LOG_DUMP__
    if(dump)
        fclose(dump);
#endif
}
