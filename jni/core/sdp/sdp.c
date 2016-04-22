#define  TAG "ZL.SDP"

#include <sdp.h>
#include <l2cap.h>
#include <zl/log.h>

static void sdp_handler(l2cap_cbk_t *cbk,l2cap_t *l2c)
{
    LOGD("sdp_handler\n");
}

void sdp_init(void)
{
    l2cap_register_protocol(SDP_PSM,sdp_handler);
}
