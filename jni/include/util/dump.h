/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 18
 ************************************************/
#ifndef __UTIL_DUMP_H__
#define __UTIL_DUMP_H__
#include <zl/types.h>
#include <stdio.h>

__BEGIN_DECLS

typedef FILE dump_t;
dump_t *new_dump(const char *path);
void dump_write(dump_t *dump,const void *buffer,size_t len);
void del_dump(dump_t *dump);

__END_DECLS
#endif

