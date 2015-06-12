/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 12
 ************************************************/
#ifndef __L2CAP_H__
#define __L2CAP_H__
#include <zl/types.h>
__BEGIN_DECLS

typedef struct {
    u16 length;
    u16 cip;
    u8  paylod[0];
} __packed l2cap_t;

void l2cap_handler(l2cap_t *l2cap);

__END_DECLS
#endif

