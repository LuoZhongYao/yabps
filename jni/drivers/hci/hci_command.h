#ifndef __HCI_COMMAND_H__
#define __HCI_COMMAND_H__
#include <zl/types.h>
__BEGIN_DECLS


void hci_inquiry(u32 lap,u8 inquiry_length,u8 num_responses);
void hci_write_scan_enable(u8 enable);
void hci_read_local_version(void);
void hci_read_buffer_size(void);
void hci_host_buffer_size(u16 acl_packet_length,u8 sco_packet_length,
        u16 acl_total_packets,u16 sco_total_packets);
void hci_read_local_features(void);

__END_DECLS
#endif
