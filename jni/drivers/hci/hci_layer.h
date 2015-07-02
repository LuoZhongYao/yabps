#ifndef __HCI_LAYER_H__
#define __HCI_LAYER_H__
#include <zl/types.h>
__BEGIN_DECLS

#include <cbk.h>

void hci_chip_active();

typedef struct {
    u16 total;
    u16 length;
    u16 handle;
    bd_addr_t bdaddr;
    void *blk;
} hci_cbk_t;

hci_cbk_t *hci_cbk_alloc(const u16 handle,const bd_addr_t *bdaddr);
hci_cbk_t *find_hci_cbk(const u16 handle);
void hci_cbk_free(hci_cbk_t *hci_cbk);

__END_DECLS
#endif
