#include <assert.h>
#include <zl/util.h>
#include <zl/log.h>
#include <transport.h>
#include <drv.h>
#include <hci.h>
#include "hci_layer.h"
typedef void (*ch_handler_t)(void *data);
typedef void (*event_handler_t)(hci_event_t *event);

static void hci_ev_command_complete(hci_event_t *ev)
{
    hci_ev_command_complete_t *msg = (hci_ev_command_complete_t *)ev;
    LOGD("pkts number %d,op_code %x,status %d",msg->num_hci_command_pkts,
            msg->op_code,msg->status);
    if(msg->op_code == HCI_NOP) {
        hci_chip_active();
    } else switch(msg->op_code) {
    case HCI_SET_HCTOHOST_FLOW_CONTROL:
        LOGD("HCI_SET_HCTOHOST_FLOW_CONTROL");
        break;

    case HCI_HOST_BUFFER_SIZE:
        LOGD("HCI_HOST_BUFFER_SIZE");
        break;

    case HCI_HOST_NUM_COMPLETED_PACKETS:
        LOGD("HCI_HOST_NUM_COMPLETED_PACKETS");
        break;

    case HCI_RESET:
        LOGD("HCI_RESET");
        break;

    case HCI_WRITE_AUTH_ENABLE:
        LOGD("HCI_WRITE_AUTH_ENABLE");
        break;

    case HCI_WRITE_ENC_MODE:
        LOGD("HCI_WRITE_ENC_MODE");
        break;

    case HCI_WRITE_SIMPLE_PAIRING_MODE:
        LOGD("HCI_WRITE_SIMPLE_PAIRING_MODE");
        break;

    case HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE:
        LOGD("HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE");
        break;

    case HCI_SET_EVENT_MASK:
        LOGD("HCI_SET_EVENT_MASK");
        break;

    case HCI_READ_LOCAL_SUPP_FEATURES:
        LOGD("HCI_READ_LOCAL_SUPP_FEATURES");
        break;

    case HCI_READ_BUFFER_SIZE:
        LOGD("HCI_READ_BUFFER_SIZE");
        break;

    case HCI_LINK_KEY_REQ_REPLY:
        LOGD("HCI_LINK_KEY_REQ_REPLY");
        break;

    case HCI_LINK_KEY_REQ_NEG_REPLY:
        LOGD("HCI_LINK_KEY_REQ_NEG_REPLY");
        break;

    case HCI_PIN_CODE_REQ_REPLY:
        LOGD("HCI_PIN_CODE_REQ_REPLY");
        break;

    case HCI_PIN_CODE_REQ_NEG_REPLY:
        LOGD("HCI_PIN_CODE_REQ_NEG_REPLY");
        break;

    case HCI_IO_CAPABILITY_RESPONSE:
        LOGD("HCI_IO_CAPABILITY_RESPONSE");
        break;

    case HCI_IO_CAPABILITY_REQUEST_NEG_REPLY:
        LOGD("HCI_IO_CAPABILITY_REQUEST_NEG_REPLY");
        break;

    case HCI_USER_CONFIRMATION_REQUEST_REPLY:
        LOGD("HCI_USER_CONFIRMATION_REQUEST_REPLY");
        break;

    case HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY:
        LOGD("HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY");
        break;

    case HCI_USER_PASSKEY_REQUEST_REPLY:
        LOGD("HCI_USER_PASSKEY_REQUEST_REPLY");
        break;

    case HCI_USER_PASSKEY_REQUEST_NEG_REPLY:
        LOGD("HCI_USER_PASSKEY_REQUEST_NEG_REPLY");
        break;

    case HCI_REMOTE_OOB_DATA_REQUEST_REPLY:
        LOGD("HCI_REMOTE_OOB_DATA_REQUEST_REPLY");
        break;

    case HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY:
        LOGD("HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY");
        break;

    case HCI_SEND_KEYPRESS_NOTIFICATION:
        LOGD("HCI_SEND_KEYPRESS_NOTIFICATION");
        break;

    case HCI_READ_LOCAL_OOB_DATA:
        LOGD("HCI_READ_LOCAL_OOB_DATA");
        break;

    case HCI_CREATE_CONNECTION_CANCEL:
        LOGD("HCI_CREATE_CONNECTION_CANCEL");
        break;

    case HCI_DELETE_STORED_LINK_KEY:
        LOGD("HCI_DELETE_STORED_LINK_KEY");
        break;

    }
}

static void hci_ev_command_status(hci_event_t *ev)
{
    hci_ev_command_status_t *msg = (hci_ev_command_status_t *)ev;
    if(msg->op_code == HCI_NOP) {
        hci_chip_active();
    } else if(msg->status != HCI_COMMAND_CURRENTLY_PENDING) switch(msg->op_code) {
    case HCI_INQUIRY:
        LOGD("HCI_INQUIRY");
        break;
    case HCI_CREATE_CONNECTION:
        LOGD("HCI_CREATE_CONNECTION");
        break;
    case HCI_DISCONNECT:
        LOGD("HCI_DISCONNECT");
        break;
    case HCI_ACCEPT_CONNECTION_REQ:
        LOGD("HCI_ACCEPT_CONNECTION_REQ");
        break;
    case HCI_REJECT_CONNECTION_REQ:
        LOGD("HCI_REJECT_CONNECTION_REQ");
        break;
    case HCI_CHANGE_CONN_PKT_TYPE:
        LOGD("HCI_CHANGE_CONN_PKT_TYPE");
        break;
    case HCI_AUTH_REQ:
        LOGD("HCI_AUTH_REQ");
        break;
    case HCI_SET_CONN_ENCRYPTION:
        LOGD("HCI_SET_CONN_ENCRYPTION");
        break;
    case HCI_CHANGE_CONN_LINK_KEY:
        LOGD("HCI_CHANGE_CONN_LINK_KEY");
        break;
    case HCI_MASTER_LINK_KEY:
        LOGD("HCI_MASTER_LINK_KEY");
        break;
    case HCI_REMOTE_NAME_REQ:
        LOGD("HCI_REMOTE_NAME_REQ");
        break;
    case HCI_READ_REMOTE_SUPP_FEATURES:
        LOGD("HCI_READ_REMOTE_SUPP_FEATURES");
        break;
    case HCI_READ_REMOTE_EXT_FEATURES:
        LOGD("HCI_READ_REMOTE_EXT_FEATURES");
        break;
    case HCI_READ_REMOTE_VER_INFO:
        LOGD("HCI_READ_REMOTE_VER_INFO");
        break;
    case HCI_READ_CLOCK_OFFSET:
        LOGD("HCI_READ_CLOCK_OFFSET");
        break;
    case HCI_HOLD_MODE:
        LOGD("HCI_HOLD_MODE");
        break;
    case HCI_SNIFF_MODE:
        LOGD("HCI_SNIFF_MODE");
        break;
    case HCI_EXIT_SNIFF_MODE:
        LOGD("HCI_EXIT_SNIFF_MODE");
        break;
    case HCI_PARK_MODE:
        LOGD("HCI_PARK_MODE");
        break;
    case HCI_EXIT_PARK_MODE:
        LOGD("HCI_EXIT_PARK_MODE");
        break;
    case HCI_QOS_SETUP:
        LOGD("HCI_QOS_SETUP");
        break;
    case HCI_SWITCH_ROLE:
        LOGD("HCI_SWITCH_ROLE");
        break;
    case HCI_FLOW_SPEC:
        LOGD("HCI_FLOW_SPEC");
        break;
    case HCI_SETUP_SYNCHRONOUS_CONN:
        LOGD("HCI_SETUP_SYNCHRONOUS_CONN");
        break;
    case HCI_ENHANCED_FLUSH:
        LOGD("HCI_ENHANCED_FLUSH");
        break;
    case HCI_REFRESH_ENCRYPTION_KEY:
        LOGD("HCI_REFRESH_ENCRYPTION_KEY");
        break;
    }
}

static const event_handler_t event_handlers[HCI_MAX_EVENT_OPCODE] = {
    [HCI_EV_COMMAND_COMPLETE] = hci_ev_command_complete,
    [HCI_EV_COMMAND_STATUS] = hci_ev_command_status,
};


static void __hci_event(struct hci_event_t *event)
{
#ifdef  HCI_DEBUG
    switch(event->event_code) {
    case HCI_EV_INQUIRY_COMPLETE:
        LOGD("HCI_EV_INQUIRY_COMPLETE");
        break;
    case HCI_EV_INQUIRY_RESULT:
        LOGD("HCI_EV_INQUIRY_RESULT");
        break;
    case HCI_EV_CONN_COMPLETE:
        LOGD("HCI_EV_CONN_COMPLETE");
        break;
    case HCI_EV_CONN_REQUEST:
        LOGD("HCI_EV_CONN_REQUEST");
        break;
    case HCI_EV_DISCONNECT_COMPLETE:
        LOGD("HCI_EV_DISCONNECT_COMPLETE");
        break;
    case HCI_EV_AUTH_COMPLETE:
        LOGD("HCI_EV_AUTH_COMPLETE");
        break;
    case HCI_EV_REMOTE_NAME_REQ_COMPLETE:
        LOGD("HCI_EV_REMOTE_NAME_REQ_COMPLETE");
        break;
    case HCI_EV_ENCRYPTION_CHANGE:
        LOGD("HCI_EV_ENCRYPTION_CHANGE");
        break;
    case HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE:
        LOGD("HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE");
        break;
    case HCI_EV_MASTER_LINK_KEY_COMPLETE:
        LOGD("HCI_EV_MASTER_LINK_KEY_COMPLETE");
        break;
    case HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE:
        LOGD("HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE");
        break;
    case HCI_EV_READ_REMOTE_VER_INFO_COMPLETE:
        LOGD("HCI_EV_READ_REMOTE_VER_INFO_COMPLETE");
        break;
    case HCI_EV_QOS_SETUP_COMPLETE:
        LOGD("HCI_EV_QOS_SETUP_COMPLETE");
        break;
    case HCI_EV_COMMAND_COMPLETE:
        LOGD("HCI_EV_COMMAND_COMPLETE");
        break;
    case HCI_EV_COMMAND_STATUS:
        LOGD("HCI_EV_COMMAND_STATUS");
        break;
    case HCI_EV_HARDWARE_ERROR:
        LOGD("HCI_EV_HARDWARE_ERROR");
        break;
    case HCI_EV_FLUSH_OCCURRED:
        LOGD("HCI_EV_FLUSH_OCCURRED");
        break;
    case HCI_EV_ROLE_CHANGE:
        LOGD("HCI_EV_ROLE_CHANGE");
        break;
    case HCI_EV_NUMBER_COMPLETED_PKTS:
        LOGD("HCI_EV_NUMBER_COMPLETED_PKTS");
        break;
    case HCI_EV_MODE_CHANGE:
        LOGD("HCI_EV_MODE_CHANGE");
        break;
    case HCI_EV_RETURN_LINK_KEYS:
        LOGD("HCI_EV_RETURN_LINK_KEYS");
        break;
    case HCI_EV_PIN_CODE_REQ:
        LOGD("HCI_EV_PIN_CODE_REQ");
        break;
    case HCI_EV_LINK_KEY_REQ:
        LOGD("HCI_EV_LINK_KEY_REQ");
        break;
    case HCI_EV_LINK_KEY_NOTIFICATION:
        LOGD("HCI_EV_LINK_KEY_NOTIFICATION");
        break;
    case HCI_EV_LOOPBACK_COMMAND:
        LOGD("HCI_EV_LOOPBACK_COMMAND");
        break;
    case HCI_EV_DATA_BUFFER_OVERFLOW:
        LOGD("HCI_EV_DATA_BUFFER_OVERFLOW");
        break;
    case HCI_EV_MAX_SLOTS_CHANGE:
        LOGD("HCI_EV_MAX_SLOTS_CHANGE");
        break;
    case HCI_EV_READ_CLOCK_OFFSET_COMPLETE:
        LOGD("HCI_EV_READ_CLOCK_OFFSET_COMPLETE");
        break;
    case HCI_EV_CONN_PACKET_TYPE_CHANGED:
        LOGD("HCI_EV_CONN_PACKET_TYPE_CHANGED");
        break;
    case HCI_EV_QOS_VIOLATION:
        LOGD("HCI_EV_QOS_VIOLATION");
        break;
    case HCI_EV_PAGE_SCAN_MODE_CHANGE:
        LOGD("HCI_EV_PAGE_SCAN_MODE_CHANGE");
        break;
    case HCI_EV_PAGE_SCAN_REP_MODE_CHANGE:
        LOGD("HCI_EV_PAGE_SCAN_REP_MODE_CHANGE");
        break;
        /* 1.2 Events */
    case HCI_EV_FLOW_SPEC_COMPLETE:
        LOGD("HCI_EV_FLOW_SPEC_COMPLETE");
        break;
    case HCI_EV_INQUIRY_RESULT_WITH_RSSI:
        LOGD("HCI_EV_INQUIRY_RESULT_WITH_RSSI");
        break;
    case HCI_EV_READ_REM_EXT_FEATURES_COMPLETE:
        LOGD("HCI_EV_READ_REM_EXT_FEATURES_COMPLETE");
        break;
    case HCI_EV_FIXED_ADDRESS:
        LOGD("HCI_EV_FIXED_ADDRESS");
        break;
    case HCI_EV_ALIAS_ADDRESS:
        LOGD("HCI_EV_ALIAS_ADDRESS");
        break;
    case HCI_EV_GENERATE_ALIAS_REQ:
        LOGD("HCI_EV_GENERATE_ALIAS_REQ");
        break;
    case HCI_EV_ACTIVE_ADDRESS:
        LOGD("HCI_EV_ACTIVE_ADDRESS");
        break;
    case HCI_EV_ALLOW_PRIVATE_PAIRING:
        LOGD("HCI_EV_ALLOW_PRIVATE_PAIRING");
        break;
    case HCI_EV_ALIAS_ADDRESS_REQ:
        LOGD("HCI_EV_ALIAS_ADDRESS_REQ");
        break;
    case HCI_EV_ALIAS_NOT_RECOGNISED:
        LOGD("HCI_EV_ALIAS_NOT_RECOGNISED");
        break;
    case HCI_EV_FIXED_ADDRESS_ATTEMPT:
        LOGD("HCI_EV_FIXED_ADDRESS_ATTEMPT");
        break;
    case HCI_EV_SYNC_CONN_COMPLETE:
        LOGD("HCI_EV_SYNC_CONN_COMPLETE");
        break;
    case HCI_EV_SYNC_CONN_CHANGED:
        LOGD("HCI_EV_SYNC_CONN_CHANGED");
        break;

        /* 2.1 Events */
    case HCI_EV_SNIFF_SUB_RATE:
        LOGD("HCI_EV_SNIFF_SUB_RATE");
        break;
    case HCI_EV_EXTENDED_INQUIRY_RESULT:
        LOGD("HCI_EV_EXTENDED_INQUIRY_RESULT");
        break;
    case HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE:
        LOGD("HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE");
        break;
    case HCI_EV_IO_CAPABILITY_REQUEST:
        LOGD("HCI_EV_IO_CAPABILITY_REQUEST");
        break;
    case HCI_EV_IO_CAPABILITY_RESPONSE:
        LOGD("HCI_EV_IO_CAPABILITY_RESPONSE");
        break;
    case HCI_EV_USER_CONFIRMATION_REQUEST:
        LOGD("HCI_EV_USER_CONFIRMATION_REQUEST");
        break;
    case HCI_EV_USER_PASSKEY_REQUEST:
        LOGD("HCI_EV_USER_PASSKEY_REQUEST");
        break;
    case HCI_EV_REMOTE_OOB_DATA_REQUEST:
        LOGD("HCI_EV_REMOTE_OOB_DATA_REQUEST");
        break;
    case HCI_EV_SIMPLE_PAIRING_COMPLETE:
        LOGD("HCI_EV_SIMPLE_PAIRING_COMPLETE");
        break;
    case HCI_EV_LST_CHANGE:
        LOGD("HCI_EV_LST_CHANGE");
        break;
    case HCI_EV_ENHANCED_FLUSH_COMPLETE:
        LOGD("HCI_EV_ENHANCED_FLUSH_COMPLETE");
        break;
    case HCI_EV_USER_PASSKEY_NOTIFICATION:
        LOGD("HCI_EV_USER_PASSKEY_NOTIFICATION");
        break;
    case HCI_EV_KEYPRESS_NOTIFICATION:
        LOGD("HCI_EV_KEYPRESS_NOTIFICATION");
        break;
    case HCI_EV_REM_HOST_SUPPORTED_FEATURES:
        LOGD("HCI_EV_REM_HOST_SUPPORTED_FEATURES");
        break;
    }
#endif
    if(event->event_code > 0 
            && event->event_code < HCI_MAX_EVENT_OPCODE
            && event_handlers[event->event_code]) {
        event_handlers[event->event_code](event);
    } else {
        LOGE("Invalid event code %d",event->event_code);
    }
}

static ch_handler_t channels [] = {
    [CHANNEL_HCI] = (void *)__hci_event,
};

int hci_receiv(transport_t channel,MessageStructure *src)
{
#ifdef HCI_DEBUG
    switch(channel){
    case CHANNEL_ACL:
        LOGD("ACL");
        break;
    case CHANNEL_SCO:
        LOGD("SCO");
        break;
    case CHANNEL_HCI:
        LOGD("HCI");
        break;
    case CHANNEL_BCCMD:
        LOGD("BCCMD");
        break;
    case CHANNEL_VM:
        LOGD("VM");
        break;
    case CHANNEL_HQ:
        LOGD("HQ");
        break;
    default:
        break;
    }
#endif
    if(channel >= CHANNEL_UNRELIABLE &&
            channel <= CHANNEL_VM &&
            channels[channel])
        channels[channel](src->buf);
    else
        LOGE("The channel(%d) can not be routed",channel);
    return 0;
}

int hci_send(transport_t channel,const void *buffer,size_t mlen)
{
    MessageStructure *msg = __new(MessageStructure);
    LOGD("abcsp send channel %d data.",channel);
    msg->buf = (void*)buffer;
    msg->buflen = mlen;
    return abcsp_sendmsg((void*)msg,channel,1);
}
