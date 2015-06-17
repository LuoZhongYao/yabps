/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 10
 ************************************************/
#ifndef __HCI_EVENT_H__
#define __HCI_EVENT_H__

#include <zl/types.h>
#include <bd_addr.h>

__BEGIN_DECLS

/******************************************************************************
   HCI The variable argumnet lists of several HCI commands are split into
   a header with pointers to pmalloc'd space. This define defines the size of
   this pmalloc space. This allows the variable argument commands to be 
   resized according to memory constraints on the host controller.
   If on XAP this in words.

 *****************************************************************************/

#define HCI_VAR_ARG_POOL_SIZE   32

/************************************************************************
 * HCI event defines ,these are events as per the HCI definetion
************************************************************************/

#define HCI_EV_INQUIRY_COMPLETE                 (0x01)
#define HCI_EV_INQUIRY_RESULT                   (0x02)
#define HCI_EV_CONN_COMPLETE                    (0x03)
#define HCI_EV_CONN_REQUEST                     (0x04)
#define HCI_EV_DISCONNECT_COMPLETE              (0x05)
#define HCI_EV_AUTH_COMPLETE                    (0x06)
#define HCI_EV_REMOTE_NAME_REQ_COMPLETE         (0x07)
#define HCI_EV_ENCRYPTION_CHANGE                (0x08)
#define HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE    (0x09)
#define HCI_EV_MASTER_LINK_KEY_COMPLETE         (0x0A)
#define HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE  (0x0B)
#define HCI_EV_READ_REMOTE_VER_INFO_COMPLETE    (0x0C)
#define HCI_EV_QOS_SETUP_COMPLETE               (0x0D)
#define HCI_EV_COMMAND_COMPLETE                 (0x0E)
#define HCI_EV_COMMAND_STATUS                   (0x0F)
#define HCI_EV_HARDWARE_ERROR                   (0x10)
#define HCI_EV_FLUSH_OCCURRED                   (0x11)
#define HCI_EV_ROLE_CHANGE                      (0x12)
#define HCI_EV_NUMBER_COMPLETED_PKTS            (0x13)
#define HCI_EV_MODE_CHANGE                      (0x14)
#define HCI_EV_RETURN_LINK_KEYS                 (0x15)
#define HCI_EV_PIN_CODE_REQ                     (0x16)
#define HCI_EV_LINK_KEY_REQ                     (0x17)
#define HCI_EV_LINK_KEY_NOTIFICATION            (0x18)
#define HCI_EV_LOOPBACK_COMMAND                 (0x19)
#define HCI_EV_DATA_BUFFER_OVERFLOW             (0x1A)
#define HCI_EV_MAX_SLOTS_CHANGE                 (0x1B)
#define HCI_EV_READ_CLOCK_OFFSET_COMPLETE       (0x1C)
#define HCI_EV_CONN_PACKET_TYPE_CHANGED         (0x1D)
#define HCI_EV_QOS_VIOLATION                    (0x1E)
#define HCI_EV_PAGE_SCAN_MODE_CHANGE            (0x1F)
#define HCI_EV_PAGE_SCAN_REP_MODE_CHANGE        (0x20)
/* 1.2 Events */
#define HCI_EV_FLOW_SPEC_COMPLETE	            (0x21)
#define HCI_EV_INQUIRY_RESULT_WITH_RSSI         (0x22)
#define HCI_EV_READ_REM_EXT_FEATURES_COMPLETE   (0x23)
#define HCI_EV_FIXED_ADDRESS                    (0x24)
#define HCI_EV_ALIAS_ADDRESS                    (0x25)
#define HCI_EV_GENERATE_ALIAS_REQ               (0x26)
#define HCI_EV_ACTIVE_ADDRESS                   (0x27)
#define HCI_EV_ALLOW_PRIVATE_PAIRING            (0x28)
#define HCI_EV_ALIAS_ADDRESS_REQ                (0x29)
#define HCI_EV_ALIAS_NOT_RECOGNISED             (0x2A)
#define HCI_EV_FIXED_ADDRESS_ATTEMPT            (0x2B)
#define HCI_EV_SYNC_CONN_COMPLETE               (0x2C)
#define HCI_EV_SYNC_CONN_CHANGED                (0x2D)

/* 2.1 Events */
#define HCI_EV_SNIFF_SUB_RATE                   (0x2E)
#define HCI_EV_EXTENDED_INQUIRY_RESULT          (0x2F)
#define HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE  (0x30)
#define HCI_EV_IO_CAPABILITY_REQUEST            (0x31)
#define HCI_EV_IO_CAPABILITY_RESPONSE           (0x32)
#define HCI_EV_USER_CONFIRMATION_REQUEST        (0x33)
#define HCI_EV_USER_PASSKEY_REQUEST             (0x34)
#define HCI_EV_REMOTE_OOB_DATA_REQUEST          (0x35)
#define HCI_EV_SIMPLE_PAIRING_COMPLETE          (0x36)
#define HCI_EV_LST_CHANGE                       (0x38)
#define HCI_EV_ENHANCED_FLUSH_COMPLETE          (0x39)
#define HCI_EV_USER_PASSKEY_NOTIFICATION        (0x3B)
#define HCI_EV_KEYPRESS_NOTIFICATION            (0x3C)
#define HCI_EV_REM_HOST_SUPPORTED_FEATURES      (0x3D)

#define HCI_MAX_EVENT_OPCODE                    (0x3E)

#define HCI_EV_MANUFACTURER_EXTENSION           (0xFF)

#define XAP_SIZE_OF_HCI_INQ_RESULT_T            10
#define HCI_BYTE_SIZE_OF_HCI_INQ_RESULT_T       14
#define HCI_MAX_INQ_RESULT_PER_PTR              (HCI_VAR_ARG_POOL_SIZE/XAP_SIZE_OF_HCI_INQ_RESULT_T)
#define HCI_MAX_INQ_RESULT_PTRS                 ( ((254 / HCI_BYTE_SIZE_OF_HCI_INQ_RESULT_T) + HCI_MAX_INQ_RESULT_PER_PTR -1) / HCI_MAX_INQ_RESULT_PER_PTR)

#define HCI_BYTE_SIZE_OF_HCI_IAC_LAP            3
#define HCI_IAC_LAP_PER_PTR                     (HCI_VAR_ARG_POOL_SIZE/HCI_ARC_CONVERT(uint24_t))
#define HCI_IAC_LAP_PTRS                        ( ((254 / HCI_BYTE_SIZE_OF_HCI_IAC_LAP) + HCI_IAC_LAP_PER_PTR -1) / HCI_IAC_LAP_PER_PTR)

#define HCI_EVENT_PKT_LENGTH                    255
#define HCI_LOOPBACK_BYTES_PER_PTR              HCI_VAR_ARG_POOL_SIZE
#define HCI_LOOPBACK_BYTE_PACKET_PTRS           ((HCI_EVENT_PKT_LENGTH + HCI_LOOPBACK_BYTES_PER_PTR -1) / HCI_LOOPBACK_BYTES_PER_PTR)

#define HCI_READ_SUPP_COMMANDS_EVENT_LENGTH     64
#define HCI_READ_SUPP_COMMANDS_PER_PTR          HCI_VAR_ARG_POOL_SIZE
#define HCI_READ_SUPP_COMMANDS_PACKET_PTRS      ((HCI_READ_SUPP_COMMANDS_EVENT_LENGTH + HCI_READ_SUPP_COMMANDS_PER_PTR - 1) / HCI_READ_SUPP_COMMANDS_PER_PTR)

#define HCI_EIR_DATA_LENGTH                     240
#define HCI_EIR_DATA_BYTES_PER_PTR              HCI_VAR_ARG_POOL_SIZE
#define HCI_EIR_DATA_PACKET_PTRS                ((HCI_EIR_DATA_LENGTH + HCI_EIR_DATA_BYTES_PER_PTR -1) / HCI_EIR_DATA_BYTES_PER_PTR)
#define HCI_EIR_DATA_LENGTH_REMAINDER           (HCI_EIR_DATA_LENGTH - (HCI_EIR_DATA_PACKET_PTRS-1) * HCI_EIR_DATA_BYTES_PER_PTR)


#define __EVENT u8 event_code;u8 param_length

typedef struct {
    __EVENT;
    u8      params[0];
} __packed hci_event_t;

typedef struct {
    __EVENT;
    u8          num_hci_command_pkts;
    u16         op_code;
    u8          status;
    u8          params[0];
}__packed hci_ev_command_complete_t;

typedef struct {
    __EVENT;
    u8          status;
    u8          num_hci_command_pkts;
    u16         op_code;
}__packed hci_ev_command_status_t;

typedef struct {
    bd_addr_t bd_addr;
    u8 page_scan_rep_mode;
    u8 page_scan_period_mode;
    u32 page_scan_mode:8;
    u32 dev_class:24;
    u16 clock_offset;

}__packed hci_inq_result_t;

typedef struct {
    __EVENT;
    u8 num_responses;
    hci_inq_result_t result[0];
}__packed hci_ev_inquiry_result_t;

typedef struct {
    __EVENT;
    bd_addr_t bd_addr;
    u32 dev_class:24;
    u32 link_type:8;
} __packed hci_ev_conn_request_t;

typedef struct {
    __EVENT;
    u8 status;
    u16 handle;
    bd_addr_t bd_addr;
    u8 link_type;
    u8 enc_enable;
}__packed hci_ev_conn_complete_t;

typedef struct {
    __EVENT;
    bd_addr_t bd_addr;
}__packed hci_ev_pin_code_req_t;

typedef struct {
    __EVENT;
    u8 status;
    u16 handle;
    u8 reason;
}__packed hci_ev_disconnect_complete_t;

typedef struct {
    __EVENT;
    u8 code;
}__packed hci_ev_hw_error_t;

__END_DECLS
#endif

