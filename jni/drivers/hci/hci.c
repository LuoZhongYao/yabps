#include <assert.h>
#include <zl/util.h>
#include <zl/log.h>
#include <transport.h>
#include <hci.h>
typedef struct hci_impl_t hci_impl_t;
typedef void (*ch_route_t)(hci_impl_t *impl,void *data);
typedef void (*event_handler_t)(hci_impl_t *impl,hci_event_t *event);

struct hci_impl_t {
    drv_t hci;
    event_handler_t event_handlers[HCI_MAX_EVENT_OPCODE];
    ch_route_t channel_route[CHANNEL_VM];
};

static void hci_ev_command_complete(hci_impl_t *impl,hci_event_t *ev)
{
    hci_ev_command_complete_t *msg = (hci_ev_command_complete_t *)ev;
    LOGD("pkts number %d,op_code %d,status %d",msg->num_hci_command_pkts,
            msg->op_code,msg->status);
}

static void hci_ev_command_status(hci_impl_t *impl,hci_event_t *ev)
{
    hci_ev_command_status_t *msg = (hci_ev_command_status_t *)ev;
    LOGD("status %d,pkts number %d,op_code %d",msg->status,
            msg->num_hci_command_pkts,msg->op_code);
}

static void __hci_event(hci_impl_t *impl,struct hci_event_t *event)
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
    if(event->event_code > 0 && event->event_code < HCI_MAX_EVENT_OPCODE) {
        impl->event_handlers[event->event_code](impl,event);
    } else {
        LOGE("Invalid event code %d",event->event_code);
    }
}

static  int __receiv(drv_t *hci,transport_channel_t channel,void *msg)
{
    hci_impl_t *impl = container_of(hci,hci_impl_t,hci);
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
    if(channel >= CHANNEL_UNRELIABLE && channel <= CHANNEL_VM)
        impl->channel_route[channel](impl,msg);
    else
        LOGE("The channel(%d) can not be routed",channel);
    return 0;
}

static int __send(drv_t *hci,transport_channel_t channel,void *msg,size_t mlen)
{
    return 0;
}

static void __hci_channel_unroute(hci_impl_t *impl __unused,void *msg __unused)
{
    LOGE("No one is to deal with this channel");
}

static void __hci_event_unhandler(hci_impl_t *impl __unused,hci_event_t *event __unused)
{
    LOGE("No one is to deal with this event,event code %d",event->event_code);
}

static int __register_event_handler(drv_t *hci,u8 event,event_handler_t handle)
{
    hci_impl_t *impl = container_of(hci,hci_impl_t,hci);
    if(event >=0 && event < HCI_MAX_EVENT_OPCODE) {
        if(impl->event_handlers[event] != __hci_event_unhandler)
            return -EEXIST;
        impl->event_handlers[event] = handle;
        return 0;
    }
    return -EINVAL;
}

static int __register_route(drv_t *hci,transport_channel_t ch ,ch_route_t handle)
{
    hci_impl_t *impl = container_of(hci,hci_impl_t,hci);
    if(ch >= CHANNEL_UNRELIABLE && ch <= CHANNEL_VM) {
        if(impl->channel_route[ch] != __hci_channel_unroute)
            return -EEXIST;
        impl->channel_route[ch] = handle;
        return 0;
    }
    return -EINVAL;
}

static int __register_handler(drv_t *hci,int type,int idx,void *handle)
{
    switch(type) {
    case HCI_CHANNEL:
        return __register_route(hci,idx,handle);
        break;
    case HCI_EVENT:
        return __register_event_handler(hci,idx,handle);
        break;
    }
    return -ENOSYS;
}


drv_t *new_hci(drv_t *top)
{
    hci_impl_t *impl = __new(hci_impl_t);
    assert(impl != NULL);
    impl->hci.send   = __send;
    impl->hci.receiv = __receiv;
    impl->hci.top = top;
    impl->hci._register = __register_handler;
    for(int i = 0;i < HCI_MAX_EVENT_OPCODE;i++)
        impl->event_handlers[i] = __hci_event_unhandler;

    for(int i = CHANNEL_UNRELIABLE;i <= CHANNEL_VM;i++)
        impl->channel_route[i] = __hci_channel_unroute;
    impl->channel_route[CHANNEL_HCI] = (ch_route_t)__hci_event;
    impl->event_handlers[HCI_EV_COMMAND_COMPLETE] = hci_ev_command_complete;
    impl->event_handlers[HCI_EV_COMMAND_STATUS] = hci_ev_command_status;
    return &(impl->hci);
}

drv_t *delete_hci(drv_t *hci)
{
    drv_t *top;
    top = hci->top;
    delete(hci);
    return top;
}
