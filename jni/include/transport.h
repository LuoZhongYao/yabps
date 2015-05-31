#ifndef __TRABSPORT_H__
#define __TRABSPORT_H__
#include <object.h>
__BEGIN_DECLS

typedef enum {
    CHANNEL_UNRELIABLE,
    CHANNEL_RELIABLE,

    CHANNEL_BCCMD ,    
    CHANNEL_HQ    ,    
    CHANNEL_DM     ,   
    CHANNEL_HCI    ,   
    CHANNEL_ACL,
    CHANNEL_SCO,
    CHANNEL_L2CAP,
    CHANNEL_RFCOMM,
    CHANNEL_SDP,
    CHANNEL_DFU,
    CHANNEL_VM,
}transport_channel_t;


__END_DECLS
#endif
