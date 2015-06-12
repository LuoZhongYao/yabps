#ifndef __HCI_DEFINES_H__
#define __HCI_DEFINES_H__
#include <zl/types.h>
#include <drv.h>
#include <abcsp/abcsp.h>
#include <hci/hci_commmand.h>
#include <hci/hci_event.h>
#include <hci/hci_error.h>
#include <blk.h>

__BEGIN_DECLS

#define HCI_MASTER          (0)
#define HCI_SLAVE           (1)

enum {
    HCI_CHANNEL,
    HCI_EVENT,
};

int hci_send(transport_t channel,const void *msg,size_t mlen);
int hci_receiv(transport_t channel,MessageStructure *src);
extern blk_t hci_blks[];

__END_DECLS

#endif
