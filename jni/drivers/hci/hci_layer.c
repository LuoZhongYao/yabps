#include <config.h>
#include "hci_layer.h"
#include <hci.h>

void hci_chip_active(void)
{
    hci_read_buffer_size();
    hci_host_buffer_size(ACL_DATA_PACKET_LENGTH,SCO_DATA_PACKET_LENGTH,
            ACL_TOTAL_PACKETS,SCO_TOTAL_PACKETS);
    hci_read_local_features();
}
