#include <hci.h>
#include <acl.h>
#include <zl/log.h>
#include "hci_layer.h"

typedef void (*event_handler_t)();

#define __STR_BDADDR    "%04x:%02x:%06x"
#define __BD_ADDR(ev)   ev->bd_addr.nap,ev->bd_addr.uap,ev->bd_addr.lap

#ifdef __LOG_HCI_EVENT__
#define __EV_STR(x)     [x] = #x
#define __CMD_STR(ocf)  #ocf    

blk_t hci_blks[MAX_ACL_CONNECTIONS] = {0};
/****************************************************************
 * OCF opcode defines - Link Control Commands
 ***************************************************************/
static const char *cmd_link_str [] = {
    NULL,
    __CMD_STR(HCI_INQUIRY                   ),
    __CMD_STR(HCI_INQUIRY_CANCEL            ),
    __CMD_STR(HCI_PERIODIC_INQUIRY_MODE     ),
    __CMD_STR(HCI_EXIT_PERIODIC_INQUIRY_MODE),
    __CMD_STR(HCI_CREATE_CONNECTION         ),
    __CMD_STR(HCI_DISCONNECT                ),
    __CMD_STR(HCI_ADD_SCO_CONNECTION        ),
    __CMD_STR(HCI_CREATE_CONNECTION_CANCEL  ),
    __CMD_STR(HCI_ACCEPT_CONNECTION_REQ     ),
    __CMD_STR(HCI_REJECT_CONNECTION_REQ     ),
    __CMD_STR(HCI_LINK_KEY_REQ_REPLY        ),
    __CMD_STR(HCI_LINK_KEY_REQ_NEG_REPLY    ),
    __CMD_STR(HCI_PIN_CODE_REQ_REPLY        ),
    __CMD_STR(HCI_PIN_CODE_REQ_NEG_REPLY    ),
    __CMD_STR(HCI_CHANGE_CONN_PKT_TYPE      ),
    __CMD_STR(HCI_AUTH_REQ                  ),
    __CMD_STR(HCI_SET_CONN_ENCRYPTION       ),
    __CMD_STR(HCI_CHANGE_CONN_LINK_KEY      ),
    __CMD_STR(HCI_MASTER_LINK_KEY           ),
    __CMD_STR(HCI_REMOTE_NAME_REQ           ),
    __CMD_STR(HCI_REMOTE_NAME_REQ_CANCEL    ),
    __CMD_STR(HCI_READ_REMOTE_SUPP_FEATURES ),
    __CMD_STR(HCI_READ_REMOTE_EXT_FEATURES  ),
    __CMD_STR(HCI_READ_REMOTE_VER_INFO      ),
    __CMD_STR(HCI_READ_CLOCK_OFFSET         ),

    /* 1.2 Features */
    __CMD_STR(HCI_READ_LMP_HANDLE              ),
    __CMD_STR(HCI_EXCHANGE_FIXED_INFO          ),
    __CMD_STR(HCI_EXCHANGE_ALIAS_INFO          ),
    __CMD_STR(HCI_PRIVATE_PAIRING_REQ_REPLY    ),
    __CMD_STR(HCI_PRIVATE_PAIRING_REQ_NEG_REPLY),
    __CMD_STR(HCI_GENERATED_ALIAS              ),
    __CMD_STR(HCI_ALIAS_ADDRESS_REQ_REPLY      ),
    __CMD_STR(HCI_ALIAS_ADDRESS_REQ_NEG_REPLY  ),
    __CMD_STR(HCI_SETUP_SYNCHRONOUS_CONN       ),
    __CMD_STR(HCI_ACCEPT_SYNCHRONOUS_CONN_REQ  ),
    __CMD_STR(HCI_REJECT_SYNCHRONOUS_CONN_REQ  ),

    /* Simple Pairing */
    __CMD_STR(HCI_IO_CAPABILITY_RESPONSE             ),
    __CMD_STR(HCI_USER_CONFIRMATION_REQUEST_REPLY    ),
    __CMD_STR(HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY),
    __CMD_STR(HCI_USER_PASSKEY_REQUEST_REPLY         ),
    __CMD_STR(HCI_USER_PASSKEY_REQUEST_NEG_REPLY     ),
    __CMD_STR(HCI_REMOTE_OOB_DATA_REQUEST_REPLY      ),
    __CMD_STR(HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY  ),
    __CMD_STR(HCI_IO_CAPABILITY_REQUEST_NEG_REPLY    ),
};
/******************************************************************************
  OP CODE defines - Link Policy Commands
 *****************************************************************************/
static const char *cmd_policy_str[] = {
    NULL,
    __CMD_STR(HCI_HOLD_MODE                 ),
    __CMD_STR(HCI_SNIFF_MODE                ),
    __CMD_STR(HCI_EXIT_SNIFF_MODE           ),
    __CMD_STR(HCI_PARK_MODE                 ),
    __CMD_STR(HCI_EXIT_PARK_MODE            ),
    __CMD_STR(HCI_QOS_SETUP                 ),
    __CMD_STR(HCI_ROLE_DISCOVERY            ),
    __CMD_STR(HCI_SWITCH_ROLE               ),
    __CMD_STR(HCI_READ_LINK_POLICY_SETTINGS ),
    __CMD_STR(HCI_WRITE_LINK_POLICY_SETTINGS),

    /* 1.2 Features */
    __CMD_STR(HCI_READ_DEFAULT_LINK_POLICY_SETTINGS ),
    __CMD_STR(HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS),
    __CMD_STR(HCI_FLOW_SPEC		                  ),
    __CMD_STR(HCI_SNIFF_SUB_RATE                    ),
};

/******************************************************************************
  OP CODE defines - Host Controller and Baseband Commands
 *****************************************************************************/
static const char *cmd_host_bb_str[] = {
    NULL,
    __CMD_STR(HCI_SET_EVENT_MASK            ),
    __CMD_STR(HCI_RESET                     ),
    __CMD_STR(HCI_SET_EVENT_FILTER          ),
    __CMD_STR(HCI_FLUSH                     ),
    __CMD_STR(HCI_READ_PIN_TYPE             ),
    __CMD_STR(HCI_WRITE_PIN_TYPE            ),
    __CMD_STR(HCI_CREATE_NEW_UNIT_KEY       ),
    __CMD_STR(HCI_READ_STORED_LINK_KEY      ),
    __CMD_STR(HCI_WRITE_STORED_LINK_KEY     ),
    __CMD_STR(HCI_DELETE_STORED_LINK_KEY    ),
    __CMD_STR(HCI_CHANGE_LOCAL_NAME         ),
    __CMD_STR(HCI_READ_LOCAL_NAME           ),
    __CMD_STR(HCI_READ_CONN_ACCEPT_TIMEOUT  ),
    __CMD_STR(HCI_WRITE_CONN_ACCEPT_TIMEOUT ),
    __CMD_STR(HCI_READ_PAGE_TIMEOUT         ),
    __CMD_STR(HCI_WRITE_PAGE_TIMEOUT        ),
    __CMD_STR(HCI_READ_SCAN_ENABLE          ),
    __CMD_STR(HCI_WRITE_SCAN_ENABLE         ),
    __CMD_STR(HCI_READ_PAGESCAN_ACTIVITY    ),
    __CMD_STR(HCI_WRITE_PAGESCAN_ACTIVITY   ),
    __CMD_STR(HCI_READ_INQUIRYSCAN_ACTIVITY ),
    __CMD_STR(HCI_WRITE_INQUIRYSCAN_ACTIVITY),
    __CMD_STR(HCI_READ_AUTH_ENABLE          ),
    __CMD_STR(HCI_WRITE_AUTH_ENABLE         ),
    __CMD_STR(HCI_READ_ENC_MODE             ),
    __CMD_STR(HCI_WRITE_ENC_MODE            ),
    __CMD_STR(HCI_READ_CLASS_OF_DEVICE      ),
    __CMD_STR(HCI_WRITE_CLASS_OF_DEVICE     ),
    __CMD_STR(HCI_READ_VOICE_SETTING        ),
    __CMD_STR(HCI_WRITE_VOICE_SETTING       ),
    __CMD_STR(HCI_READ_AUTO_FLUSH_TIMEOUT   ),
    __CMD_STR(HCI_WRITE_AUTO_FLUSH_TIMEOUT  ),
    __CMD_STR(HCI_READ_NUM_BCAST_RETXS      ),
    __CMD_STR(HCI_WRITE_NUM_BCAST_RETXS     ),
    __CMD_STR(HCI_READ_HOLD_MODE_ACTIVITY   ),
    __CMD_STR(HCI_WRITE_HOLD_MODE_ACTIVITY  ),
    __CMD_STR(HCI_READ_TX_POWER_LEVEL       ),
    __CMD_STR(HCI_READ_SCO_FLOW_CON_ENABLE  ),
    __CMD_STR(HCI_WRITE_SCO_FLOW_CON_ENABLE ),
    __CMD_STR(HCI_SET_HCTOHOST_FLOW_CONTROL ),
    __CMD_STR(HCI_HOST_BUFFER_SIZE          ),
    __CMD_STR(HCI_HOST_NUM_COMPLETED_PACKETS),
    __CMD_STR(HCI_READ_LINK_SUPERV_TIMEOUT  ),
    __CMD_STR(HCI_WRITE_LINK_SUPERV_TIMEOUT ),
    __CMD_STR(HCI_READ_NUM_SUPPORTED_IAC    ),
    __CMD_STR(HCI_READ_CURRENT_IAC_LAP      ),
    __CMD_STR(HCI_WRITE_CURRENT_IAC_LAP     ),
    __CMD_STR(HCI_READ_PAGESCAN_PERIOD_MODE ),
    __CMD_STR(HCI_WRITE_PAGESCAN_PERIOD_MODE),
    __CMD_STR(HCI_READ_PAGESCAN_MODE        ),
    __CMD_STR(HCI_WRITE_PAGESCAN_MODE       ),

    /* 1.2 Features */
    __CMD_STR(HCI_SET_AFH_CHANNEL_CLASS     ),
    __CMD_STR(HCI_READ_INQUIRY_SCAN_TYPE    ),
    __CMD_STR(HCI_WRITE_INQUIRY_SCAN_TYPE   ),
    __CMD_STR(HCI_READ_INQUIRY_MODE         ),
    __CMD_STR(HCI_WRITE_INQUIRY_MODE        ),
    __CMD_STR(HCI_READ_PAGE_SCAN_TYPE       ),
    __CMD_STR(HCI_WRITE_PAGE_SCAN_TYPE      ),
    __CMD_STR(HCI_READ_AFH_CHANNEL_CLASS_M  ),
    __CMD_STR(HCI_WRITE_AFH_CHANNEL_CLASS_M ),
    __CMD_STR(HCI_READ_ANON_MODE            ),
    __CMD_STR(HCI_WRITE_ANON_MODE           ),
    __CMD_STR(HCI_READ_ALIAS_AUTH_ENABLE    ),
    __CMD_STR(HCI_WRITE_ALIAS_AUTH_ENABLE   ),
    __CMD_STR(HCI_READ_ANON_ADDR_CHANGE_PARAMS ),
    __CMD_STR(HCI_WRITE_ANON_ADDR_CHANGE_PARAMS),
    __CMD_STR(HCI_RESET_FIXED_ADDRESS_ATTEMPTS_COUNTER ),

    /* 2.1 Features */
    __CMD_STR(HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA  ),
    __CMD_STR(HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA ),
    __CMD_STR(HCI_REFRESH_ENCRYPTION_KEY               ),
    __CMD_STR(HCI_READ_SIMPLE_PAIRING_MODE             ),
    __CMD_STR(HCI_WRITE_SIMPLE_PAIRING_MODE            ),
    __CMD_STR(HCI_READ_LOCAL_OOB_DATA                  ),
    __CMD_STR(HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL ),
    __CMD_STR(HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL   ),

    __CMD_STR(HCI_ENHANCED_FLUSH            ),
    __CMD_STR(HCI_SEND_KEYPRESS_NOTIFICATION),
};

/******************************************************************************
  OP CODE defines - Informational Parameters
 *****************************************************************************/
static const char *cmd_info_str[] = {
    NULL,
    __CMD_STR(HCI_READ_LOCAL_VER_INFO       ),
    /* 1.2 Feature */
    __CMD_STR(HCI_READ_LOCAL_SUPP_COMMANDS  ),
    __CMD_STR(HCI_READ_LOCAL_SUPP_FEATURES  ),
    /* 1.2 Feature */
    __CMD_STR(HCI_READ_LOCAL_EXT_FEATURES   ),
    __CMD_STR(HCI_READ_BUFFER_SIZE          ),
    __CMD_STR(HCI_READ_COUNTRY_CODE         ),
    __CMD_STR(HCI_READ_BD_ADDR              ),
};

/******************************************************************************
  OP CODE defines - Status Parameters
 *****************************************************************************/
static const char *cmd_status_str[] = {
    NULL,
    __CMD_STR(HCI_READ_FAILED_CONTACT_COUNT ),
    __CMD_STR(HCI_RESET_FAILED_CONTACT_COUNT),
    __CMD_STR(HCI_GET_LINK_QUALITY          ),
    __CMD_STR(HCI_READ_RSSI                 ),

    /* 1.2 Features */
    __CMD_STR(HCI_READ_AFH_CHANNEL_MAP      ),
    __CMD_STR(HCI_READ_CLOCK                ),
};

/******************************************************************************
  OP CODE defines - Testing Parameters
 *****************************************************************************/
static const char *cmd_test_str[] = {
    NULL,
    __CMD_STR(HCI_READ_LOOPBACK_MODE              ),
    __CMD_STR(HCI_WRITE_LOOPBACK_MODE             ),
    __CMD_STR(HCI_ENABLE_DUT_MODE                 ),
    __CMD_STR(HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE ),
};

static const char **command_strings[] = {
    NULL,cmd_link_str,cmd_policy_str,cmd_host_bb_str,
    cmd_info_str,cmd_status_str,cmd_test_str,
};

static const char *event_strings[] = {
    __EV_STR(HCI_EV_INQUIRY_COMPLETE),
    __EV_STR(HCI_EV_INQUIRY_RESULT),
    __EV_STR(HCI_EV_CONN_COMPLETE),
    __EV_STR(HCI_EV_CONN_REQUEST),
    __EV_STR(HCI_EV_DISCONNECT_COMPLETE),
    __EV_STR(HCI_EV_AUTH_COMPLETE),
    __EV_STR(HCI_EV_REMOTE_NAME_REQ_COMPLETE),
    __EV_STR(HCI_EV_ENCRYPTION_CHANGE),
    __EV_STR(HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE),
    __EV_STR(HCI_EV_MASTER_LINK_KEY_COMPLETE),
    __EV_STR(HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE),
    __EV_STR(HCI_EV_READ_REMOTE_VER_INFO_COMPLETE),
    __EV_STR(HCI_EV_QOS_SETUP_COMPLETE),
    __EV_STR(HCI_EV_COMMAND_COMPLETE),
    __EV_STR(HCI_EV_COMMAND_STATUS),
    __EV_STR(HCI_EV_HARDWARE_ERROR),
    __EV_STR(HCI_EV_FLUSH_OCCURRED),
    __EV_STR(HCI_EV_ROLE_CHANGE),
    __EV_STR(HCI_EV_NUMBER_COMPLETED_PKTS),
    __EV_STR(HCI_EV_MODE_CHANGE),
    __EV_STR(HCI_EV_RETURN_LINK_KEYS),
    __EV_STR(HCI_EV_PIN_CODE_REQ),
    __EV_STR(HCI_EV_LINK_KEY_REQ),
    __EV_STR(HCI_EV_LINK_KEY_NOTIFICATION),
    __EV_STR(HCI_EV_LOOPBACK_COMMAND),
    __EV_STR(HCI_EV_DATA_BUFFER_OVERFLOW),
    __EV_STR(HCI_EV_MAX_SLOTS_CHANGE),
    __EV_STR(HCI_EV_READ_CLOCK_OFFSET_COMPLETE),
    __EV_STR(HCI_EV_CONN_PACKET_TYPE_CHANGED),
    __EV_STR(HCI_EV_QOS_VIOLATION),
    __EV_STR(HCI_EV_PAGE_SCAN_MODE_CHANGE),
    __EV_STR(HCI_EV_PAGE_SCAN_REP_MODE_CHANGE),
    /* 1.2 Events */
    __EV_STR(HCI_EV_FLOW_SPEC_COMPLETE	),
    __EV_STR(HCI_EV_INQUIRY_RESULT_WITH_RSSI),
    __EV_STR(HCI_EV_READ_REM_EXT_FEATURES_COMPLETE),
    __EV_STR(HCI_EV_FIXED_ADDRESS),
    __EV_STR(HCI_EV_ALIAS_ADDRESS),
    __EV_STR(HCI_EV_GENERATE_ALIAS_REQ),
    __EV_STR(HCI_EV_ACTIVE_ADDRESS),
    __EV_STR(HCI_EV_ALLOW_PRIVATE_PAIRING),
    __EV_STR(HCI_EV_ALIAS_ADDRESS_REQ),
    __EV_STR(HCI_EV_ALIAS_NOT_RECOGNISED),
    __EV_STR(HCI_EV_FIXED_ADDRESS_ATTEMPT),
    __EV_STR(HCI_EV_SYNC_CONN_COMPLETE),
    __EV_STR(HCI_EV_SYNC_CONN_CHANGED),
    /* 2.1 Events */
    __EV_STR(HCI_EV_SNIFF_SUB_RATE),
    __EV_STR(HCI_EV_EXTENDED_INQUIRY_RESULT),
    __EV_STR(HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE),
    __EV_STR(HCI_EV_IO_CAPABILITY_REQUEST),
    __EV_STR(HCI_EV_IO_CAPABILITY_RESPONSE),
    __EV_STR(HCI_EV_USER_CONFIRMATION_REQUEST),
    __EV_STR(HCI_EV_USER_PASSKEY_REQUEST),
    __EV_STR(HCI_EV_REMOTE_OOB_DATA_REQUEST),
    __EV_STR(HCI_EV_SIMPLE_PAIRING_COMPLETE),
    __EV_STR(HCI_EV_LST_CHANGE),
    __EV_STR(HCI_EV_ENHANCED_FLUSH_COMPLETE),
    __EV_STR(HCI_EV_USER_PASSKEY_NOTIFICATION),
    __EV_STR(HCI_EV_KEYPRESS_NOTIFICATION),
    __EV_STR(HCI_EV_REM_HOST_SUPPORTED_FEATURES),
};
#endif
static void hci_ev_command_complete(hci_event_t *ev)
{
    hci_ev_command_complete_t *msg = (hci_ev_command_complete_t *)ev;
    if(msg->op_code == HCI_NOP) {
#ifdef __LOG_HCI_EVENT__
        LOGD("[C] NOP Command Complete,chip active");
#endif
        hci_chip_active();
    } else {
#ifdef __LOG_HCI_EVENT__
        LOGD("[C] %s Command %s,pkts number %d",
                command_strings[msg->op_code >> 10][msg->op_code & HCI_OPCODE_MASK],
                hci_error_string(msg->status),msg->num_hci_command_pkts);
#endif
    } 
    switch(msg->op_code) {
    case HCI_SET_HCTOHOST_FLOW_CONTROL:
        break;

    case HCI_HOST_BUFFER_SIZE:
        break;

    case HCI_HOST_NUM_COMPLETED_PACKETS:
        break;

    case HCI_RESET:
        break;

    case HCI_WRITE_AUTH_ENABLE:
        break;

    case HCI_WRITE_ENC_MODE:
        break;

    case HCI_WRITE_SIMPLE_PAIRING_MODE:
        break;

    case HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE:
        break;

    case HCI_SET_EVENT_MASK:
        break;

    case HCI_READ_LOCAL_SUPP_FEATURES:
        break;

    case HCI_READ_BUFFER_SIZE:
        hci_write_scan_enable(HCI_SCAN_ENABLE_INQ_AND_PAGE);
        break;

    case HCI_LINK_KEY_REQ_REPLY:
        break;

    case HCI_LINK_KEY_REQ_NEG_REPLY:
        break;

    case HCI_PIN_CODE_REQ_REPLY:
        break;

    case HCI_PIN_CODE_REQ_NEG_REPLY:
        break;

    case HCI_IO_CAPABILITY_RESPONSE:
        break;

    case HCI_IO_CAPABILITY_REQUEST_NEG_REPLY:
        break;

    case HCI_USER_CONFIRMATION_REQUEST_REPLY:
        break;

    case HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY:
        break;

    case HCI_USER_PASSKEY_REQUEST_REPLY:
        break;

    case HCI_USER_PASSKEY_REQUEST_NEG_REPLY:
        break;

    case HCI_REMOTE_OOB_DATA_REQUEST_REPLY:
        break;

    case HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY:
        break;

    case HCI_SEND_KEYPRESS_NOTIFICATION:
        break;

    case HCI_READ_LOCAL_OOB_DATA:
        break;

    case HCI_CREATE_CONNECTION_CANCEL:
        break;

    case HCI_DELETE_STORED_LINK_KEY:
        break;
    default:
        hci_inquiry(0x9e8b33,HCI_INQUIRY_LENGTH_MAX,0);
        break;
    }
}

static void hci_ev_command_status(hci_event_t *ev)
{
    hci_ev_command_status_t *msg = (hci_ev_command_status_t *)ev;
    if(msg->op_code == HCI_NOP) {
#ifdef __LOG_HCI_EVENT__
        LOGD("[S] NOP Command Status,Active Chip");
#endif
        hci_chip_active();
    } else {
#ifdef __LOG_HCI_EVENT__
        LOGD("[S] %s Command %s,pkts number %d",
                command_strings[msg->op_code >> 10][msg->op_code & HCI_OPCODE_MASK],
                hci_error_string(msg->status),msg->num_hci_command_pkts);
#endif
    } 
}

static void hci_ev_inquiry_result(hci_ev_inquiry_result_t *ev)
{
    u8 i = 0;
    LOGD("number : %d,length %d",ev->num_responses,ev->param_length);
    for(i = 0;i < ev->num_responses;i++){
        hci_inq_result_t *result = ev->result + i;
        LOGD("%04x:%2x:%06x mode %x,class : %x,clock_offset %x",result->bd_addr.nap,
                result->bd_addr.uap,result->bd_addr.lap,result->page_scan_rep_mode,
                result->dev_class,result->clock_offset);
    }
}

static void hci_ev_conn_complete(hci_ev_conn_complete_t *ev)
{
    blk_t *blk;
    LOGD("%04x:%02x:%06x Connect Complete %s,handle %x link type %x"
            "encryption enable %x",
            ev->bd_addr.nap,ev->bd_addr.nap,ev->bd_addr.lap,
            hci_error_string(ev->status),ev->handle,ev->link_type,ev->enc_enable);
    blk = alloc_blk(hci_blks,MAX_ACL_CONNECTIONS);
    assert(blk);
    blk->handle = ev->handle;
}

static void hci_ev_disconnect_complete(hci_ev_disconnect_complete_t *ev)
{
    LOGD("Disconnect Complete %s,handle %x,reason %x",
            hci_error_string(ev->status),ev->handle,ev->reason);
    free_blk(get_blk(ev->handle,hci_blks,MAX_ACL_CONNECTIONS));
}

static void hci_ev_conn_request(hci_ev_conn_request_t *ev)
{
    //bd_addr_t bd_addr = {0xa11cf3,0x18,0x000d};
    LOGD("%04x:%02x:%06x device class : %x link type %x",
            ev->bd_addr.nap,ev->bd_addr.uap,ev->bd_addr.lap,
            ev->dev_class,ev->link_type);
    hci_accept_connection(&(ev->bd_addr),HCI_MASTER);
}

static void hci_ev_pin_code_req(hci_ev_pin_code_req_t *ev)
{
    LOGD("%04x:%02x:%06x",ev->bd_addr.nap,ev->bd_addr.uap,
            ev->bd_addr.lap);
    hci_pin_code_req_reply(&(ev->bd_addr),4,(u8*)"0000");
}

static const event_handler_t event_handlers[HCI_MAX_EVENT_OPCODE] = {
    [HCI_EV_INQUIRY_RESULT]     = hci_ev_inquiry_result,
    [HCI_EV_CONN_COMPLETE]      = hci_ev_conn_complete,
    [HCI_EV_CONN_REQUEST]       = hci_ev_conn_request,
    [HCI_EV_DISCONNECT_COMPLETE]= hci_ev_disconnect_complete,
    [HCI_EV_PIN_CODE_REQ]       = hci_ev_pin_code_req,
    [HCI_EV_COMMAND_COMPLETE]   = hci_ev_command_complete,
    [HCI_EV_COMMAND_STATUS]     = hci_ev_command_status,
};

void hci_event_handler(hci_event_t *event)
{
    if(event->event_code > 0 
            && event->event_code < HCI_MAX_EVENT_OPCODE
      ){
#ifdef __LOG_HCI_EVENT__
        LOGD("[E] %s",event_strings[event->event_code]);
#endif
        if(event_handlers[event->event_code]) {
            event_handlers[event->event_code](event);
        } else {
            LOGE("[E] Unhandle Event %d",event->event_code);
        }
    } else {
        LOGE("[E] Invalid event code %d",event->event_code);
    }
}
