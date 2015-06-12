#include <zl/types.h>
#include <zl/log.h>
#include <l2cap.h>

void l2cap_handler(l2cap_t *l2cap)
{
    LOGD("legnth %x,cip %x",l2cap->length,l2cap->cip);
}
