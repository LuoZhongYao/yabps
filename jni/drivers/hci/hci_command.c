#include <hci.h>
#include <zl/util.h>
#include <assert.h>

#define __DECAL(t,code)  t *cmd = __new(t);assert(cmd);cmd->op_code = code;cmd->length = (sizeof(t) - sizeof(hci_command_t))
#define __SEND(t)   hci_send(CHANNEL_HCI,cmd,sizeof(*cmd))

void hci_inquiry(u32 lap,u8 inquiry_length,u8 num_responses)
{
    __DECAL(hci_inquiry_t,HCI_INQUIRY);
    cmd->lap = lap;
    cmd->inquiry_length = inquiry_length;
    cmd->responses = num_responses;
    __SEND();
}

void hci_write_scan_enable(u8 enable)
{
    __DECAL(hci_write_scan_enable_t,HCI_WRITE_SCAN_ENABLE);
    cmd->scan_enable = enable;
    __SEND();
}

void hci_read_local_version(void)
{
    __DECAL(hci_read_local_version_t,HCI_READ_LOCAL_VER_INFO);
    __SEND();
}

void hci_read_buffer_size(void)
{
    __DECAL(hci_read_buffer_size_t,HCI_READ_BUFFER_SIZE);
    __SEND();
}

void hci_host_buffer_size(u16 acl_packet_length,u8 sco_packet_length,
        u16 acl_total_packets,u16 sco_total_packets)
{
    __DECAL(hci_host_buffer_size_t,HCI_HOST_BUFFER_SIZE);
    cmd->acl_packet_length = acl_packet_length;
    cmd->sco_packet_length = sco_packet_length;
    cmd->acl_total_packets = acl_total_packets;
    cmd->sco_total_packets = sco_total_packets;
    __SEND();
}

void hci_read_local_features(void)
{
    __DECAL(hci_read_local_features_t,HCI_READ_LOCAL_SUPP_FEATURES);
    __SEND();
}
