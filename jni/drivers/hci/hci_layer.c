#include <config.h>
#include "hci_layer.h"
#include <hci.h>
#include <assert.h>

static LIST_HEAD(hci_links);
#define cbk_of(ptr) ({\
    const void *__mptr = (ptr);\
    (cbk_t*)((u8 *)__mptr - offsetof(cbk_t,private_data));})

void hci_chip_active(void)
{
    hci_read_buffer_size();
    hci_host_buffer_size(ACL_DATA_PACKET_LENGTH,SCO_DATA_PACKET_LENGTH,
            ACL_TOTAL_PACKETS,SCO_TOTAL_PACKETS);
    hci_read_local_features();
}

hci_cbk_t *hci_cbk_alloc(const u16 handle,const bd_addr_t *bdaddr)
{
    cbk_t *cbk;
    hci_cbk_t *hci_cbk;
    cbk = cbk_alloc(&hci_links,sizeof(*hci_cbk));
    assert(cbk);
    hci_cbk = (hci_cbk_t *)cbk->private_data;
    hci_cbk->handle = handle;
    hci_cbk->bdaddr = *bdaddr;
    hci_cbk->length = 0;
    hci_cbk->total  = 0;
    hci_cbk->blk    = NULL;
    return hci_cbk;
}

void hci_cbk_free(hci_cbk_t *hci_cbk)
{
    cbk_free(cbk_of(hci_cbk));
}

static bool cmp_handle(const void *pos,const void *who)
{
    const hci_cbk_t *hci_cbk = pos;
    return (hci_cbk->handle == *((const u16 *)who));
}

hci_cbk_t *find_hci_cbk(const u16 handle) 
{
    cbk_t *cbk;
    cbk = find_cbk(&hci_links,&handle,cmp_handle);
    if(cbk)
        return (hci_cbk_t *)cbk->private_data;
    return NULL;
}
