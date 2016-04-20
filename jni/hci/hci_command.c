#include <hci.h>
#include <zl/util.h>
#include <assert.h>

#define __DECAL(t,code,len) \
    t *cmd = __alloc(sizeof(t) + len);\
assert(cmd);\
cmd->op_code = code;\
cmd->length = (sizeof(t) - sizeof(hci_command_t) + len)
#define __SEND(n)   hci_send(CHANNEL_HCI,cmd,sizeof(*cmd) + n)

void hci_inquiry(u32 lap,u8 inquiry_length,u8 num_responses)
{
    __DECAL(hci_inquiry_t,HCI_INQUIRY,0);
    cmd->lap = lap;
    cmd->inquiry_length = inquiry_length;
    cmd->responses = num_responses;
    __SEND(0);
}

void hci_accept_connection(bd_addr_t *bd_addr,u8 role)
{
    __DECAL(hci_accept_connection_req_t,HCI_ACCEPT_CONNECTION_REQ,0);
    cmd->bd_addr = *bd_addr;
    cmd->role = role;
    __SEND(0);
}

void hci_pin_code_req_reply(bd_addr_t *bd_addr,u8 pin_length,u8 *pin)
{
    __DECAL(hci_pin_code_req_reply_t,HCI_PIN_CODE_REQ_REPLY,0);
    cmd->pin_length = pin_length > HCI_MAX_PIN_LENGTH ? HCI_MAX_PIN_LENGTH : pin_length;
    cmd->bd_addr = *bd_addr;
    memcpy(cmd->pin,pin,cmd->pin_length);
    __SEND(0);
}

void hci_write_scan_enable(u8 enable)
{
    __DECAL(hci_write_scan_enable_t,HCI_WRITE_SCAN_ENABLE,0);
    cmd->scan_enable = enable;
    __SEND(0);
}

void hci_read_local_version(void)
{
    __DECAL(hci_read_local_version_t,HCI_READ_LOCAL_VER_INFO,0);
    __SEND(0);
}

void hci_read_buffer_size(void)
{
    __DECAL(hci_read_buffer_size_t,HCI_READ_BUFFER_SIZE,0);
    __SEND(0);
}

void hci_host_buffer_size(u16 acl_packet_length,u8 sco_packet_length,
        u16 acl_total_packets,u16 sco_total_packets)
{
    __DECAL(hci_host_buffer_size_t,HCI_HOST_BUFFER_SIZE,0);
    cmd->acl_packet_length = acl_packet_length;
    cmd->sco_packet_length = sco_packet_length;
    cmd->acl_total_packets = acl_total_packets;
    cmd->sco_total_packets = sco_total_packets;
    __SEND(0);
}

void hci_read_local_features(void)
{
    __DECAL(hci_read_local_features_t,HCI_READ_LOCAL_SUPP_FEATURES,0);
    __SEND(0);
}

void hci_write_local_name(const u8 *name,u16 len)
{
    __DECAL(hci_write_local_name_t,HCI_WRITE_LOCAL_NAME,0);
    len = min(len,HCI_MAX_LOCAL_NAME_LENGTH);
    memcpy(cmd->name,name,len);
    __SEND(0);
}

void hci_read_local_name(void)
{
    __DECAL(hci_read_local_name_t,HCI_READ_LOCAL_NAME,0);
    __SEND(0);
}
