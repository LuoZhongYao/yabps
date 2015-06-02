#ifndef __HCI_DEFINES_H__
#define __HCI_DEFINES_H__
#include <zl/types.h>
#include <drv.h>
__BEGIN_DECLS

/************************************************************************
 * Defines for the OGF opcodes group 
 ************************************************************************/
#define HCI_OGF_BIT_OFFSET  (10)

#define HCI_LINK                       (0x0400)
#define HCI_POLICY                     (0x0800)
#define HCI_HOST_BB                    (0x0C00)
#define HCI_INFO                       (0x1000)
#define HCI_STATUS                     (0x1400)
#define HCI_TEST                       (0x1800)
#define HCI_MAX_OGF                    (0x1C00)
#define HCI_MANUFACTURER_EXTENSION     (0xFC00)
#define HCI_OPCODE_GROUP_MASK          (0xFC00)
#define HCI_OPCODE_MASK                (0x03FF)

/*---------------------------------------------------------------------------*\
   These defines are needed so that the hci generator can identify
   the reply (_RET) macros.
\*---------------------------------------------------------------------------*/
#define HCI_LINK_RET                   HCI_LINK
#define HCI_POLICY_RET                 HCI_POLICY
#define HCI_HOST_BB_RET                HCI_HOST_BB
#define HCI_INFO_RET                   HCI_INFO
#define HCI_STATUS_RET                 HCI_STATUS
#define HCI_TEST_RET                   HCI_TEST
 
/******************************************************************************
   Miscellaneous
 *****************************************************************************/
#define HCI_NOP                          0x0000

/******************************************************************************
   OCF opcode defines - Link Control Commands
 *****************************************************************************/
#define HCI_INQUIRY                    (HCI_LINK | 0x0001)
#define HCI_INQUIRY_CANCEL             (HCI_LINK | 0x0002)
#define HCI_PERIODIC_INQUIRY_MODE      (HCI_LINK | 0x0003)
#define HCI_EXIT_PERIODIC_INQUIRY_MODE (HCI_LINK | 0x0004)
#define HCI_CREATE_CONNECTION          (HCI_LINK | 0x0005)
#define HCI_DISCONNECT                 (HCI_LINK | 0x0006)
#define HCI_ADD_SCO_CONNECTION         (HCI_LINK | 0x0007)
#define HCI_CREATE_CONNECTION_CANCEL   (HCI_LINK | 0x0008)
#define HCI_ACCEPT_CONNECTION_REQ      (HCI_LINK | 0x0009)
#define HCI_REJECT_CONNECTION_REQ      (HCI_LINK | 0x000A)
#define HCI_LINK_KEY_REQ_REPLY         (HCI_LINK | 0x000B)
#define HCI_LINK_KEY_REQ_NEG_REPLY     (HCI_LINK | 0x000C)
#define HCI_PIN_CODE_REQ_REPLY         (HCI_LINK | 0x000D)
#define HCI_PIN_CODE_REQ_NEG_REPLY     (HCI_LINK | 0x000E)
#define HCI_CHANGE_CONN_PKT_TYPE       (HCI_LINK | 0x000F)
#define HCI_AUTH_REQ                   (HCI_LINK | 0x0011)
#define HCI_SET_CONN_ENCRYPTION        (HCI_LINK | 0x0013)
#define HCI_CHANGE_CONN_LINK_KEY       (HCI_LINK | 0x0015)
#define HCI_MASTER_LINK_KEY            (HCI_LINK | 0x0017)
#define HCI_REMOTE_NAME_REQ            (HCI_LINK | 0x0019)
#define HCI_REMOTE_NAME_REQ_CANCEL     (HCI_LINK | 0x001A)
#define HCI_READ_REMOTE_SUPP_FEATURES  (HCI_LINK | 0x001B)
#define HCI_READ_REMOTE_EXT_FEATURES   (HCI_LINK | 0x001C)
#define HCI_READ_REMOTE_VER_INFO       (HCI_LINK | 0x001D)
#define HCI_READ_CLOCK_OFFSET          (HCI_LINK | 0x001F)
#define HCI_MAX_LINK_OCF_V1_1          ( 0x0020)

/* 1.2 Features */
#define HCI_READ_LMP_HANDLE               (HCI_LINK | 0x0020)
#define HCI_EXCHANGE_FIXED_INFO           (HCI_LINK | 0x0021)
#define HCI_EXCHANGE_ALIAS_INFO           (HCI_LINK | 0x0022)
#define HCI_PRIVATE_PAIRING_REQ_REPLY     (HCI_LINK | 0x0023)
#define HCI_PRIVATE_PAIRING_REQ_NEG_REPLY (HCI_LINK | 0x0024)
#define HCI_GENERATED_ALIAS               (HCI_LINK | 0x0025)
#define HCI_ALIAS_ADDRESS_REQ_REPLY       (HCI_LINK | 0x0026)
#define HCI_ALIAS_ADDRESS_REQ_NEG_REPLY   (HCI_LINK | 0x0027)
#define HCI_SETUP_SYNCHRONOUS_CONN        (HCI_LINK | 0x0028)
#define HCI_ACCEPT_SYNCHRONOUS_CONN_REQ   (HCI_LINK | 0x0029)
#define HCI_REJECT_SYNCHRONOUS_CONN_REQ   (HCI_LINK | 0x002A)

/* Simple Pairing */
#define HCI_IO_CAPABILITY_RESPONSE              (HCI_LINK | 0x002B)
#define HCI_USER_CONFIRMATION_REQUEST_REPLY     (HCI_LINK | 0x002C)
#define HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY (HCI_LINK | 0x002D)
#define HCI_USER_PASSKEY_REQUEST_REPLY          (HCI_LINK | 0x002E)
#define HCI_USER_PASSKEY_REQUEST_NEG_REPLY      (HCI_LINK | 0x002F)
#define HCI_REMOTE_OOB_DATA_REQUEST_REPLY       (HCI_LINK | 0x0030)
#define HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY   (HCI_LINK | 0x0033)
#define HCI_IO_CAPABILITY_REQUEST_NEG_REPLY     (HCI_LINK | 0x0034)


#define HCI_MAX_LINK_OCF               ( 0x0035)

/******************************************************************************
   OP CODE defines - Link Policy Commands
 *****************************************************************************/
#define HCI_HOLD_MODE                  (HCI_POLICY | 0x0001)
#define HCI_SNIFF_MODE                 (HCI_POLICY | 0x0003)
#define HCI_EXIT_SNIFF_MODE            (HCI_POLICY | 0x0004)
#define HCI_PARK_MODE                  (HCI_POLICY | 0x0005)
#define HCI_EXIT_PARK_MODE             (HCI_POLICY | 0x0006)
#define HCI_QOS_SETUP                  (HCI_POLICY | 0x0007)
#define HCI_ROLE_DISCOVERY             (HCI_POLICY | 0x0009)
#define HCI_SWITCH_ROLE                (HCI_POLICY | 0x000B)
#define HCI_READ_LINK_POLICY_SETTINGS  (HCI_POLICY | 0x000C)
#define HCI_WRITE_LINK_POLICY_SETTINGS (HCI_POLICY | 0x000D)
#define HCI_MAX_POLICY_OCF_V1_1        ( 0x000E)

/* 1.2 Features */
#define HCI_READ_DEFAULT_LINK_POLICY_SETTINGS  (HCI_POLICY | 0x000E)
#define HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS (HCI_POLICY | 0x000F)
#define HCI_FLOW_SPEC		       (HCI_POLICY | 0x0010)
#define HCI_SNIFF_SUB_RATE             (HCI_POLICY | 0x0011)
#define HCI_MAX_POLICY_OCF             ( 0x0012)

/******************************************************************************
   OP CODE defines - Host Controller and Baseband Commands
 *****************************************************************************/
#define HCI_SET_EVENT_MASK             (HCI_HOST_BB | 0x0001)
#define HCI_RESET                      (HCI_HOST_BB | 0x0003)
#define HCI_SET_EVENT_FILTER           (HCI_HOST_BB | 0x0005)
#define HCI_FLUSH                      (HCI_HOST_BB | 0x0008)
#define HCI_READ_PIN_TYPE              (HCI_HOST_BB | 0x0009)
#define HCI_WRITE_PIN_TYPE             (HCI_HOST_BB | 0x000A)
#define HCI_CREATE_NEW_UNIT_KEY        (HCI_HOST_BB | 0x000B)
#define HCI_READ_STORED_LINK_KEY       (HCI_HOST_BB | 0x000D)
#define HCI_WRITE_STORED_LINK_KEY      (HCI_HOST_BB | 0x0011)
#define HCI_DELETE_STORED_LINK_KEY     (HCI_HOST_BB | 0x0012)
#define HCI_CHANGE_LOCAL_NAME          (HCI_HOST_BB | 0x0013)
#define HCI_READ_LOCAL_NAME            (HCI_HOST_BB | 0x0014)
#define HCI_READ_CONN_ACCEPT_TIMEOUT   (HCI_HOST_BB | 0x0015)
#define HCI_WRITE_CONN_ACCEPT_TIMEOUT  (HCI_HOST_BB | 0x0016)
#define HCI_READ_PAGE_TIMEOUT          (HCI_HOST_BB | 0x0017)
#define HCI_WRITE_PAGE_TIMEOUT         (HCI_HOST_BB | 0x0018)
#define HCI_READ_SCAN_ENABLE           (HCI_HOST_BB | 0x0019)
#define HCI_WRITE_SCAN_ENABLE          (HCI_HOST_BB | 0x001A)
#define HCI_READ_PAGESCAN_ACTIVITY     (HCI_HOST_BB | 0x001B)
#define HCI_WRITE_PAGESCAN_ACTIVITY    (HCI_HOST_BB | 0x001C)
#define HCI_READ_INQUIRYSCAN_ACTIVITY  (HCI_HOST_BB | 0x001D)
#define HCI_WRITE_INQUIRYSCAN_ACTIVITY (HCI_HOST_BB | 0x001E)
#define HCI_READ_AUTH_ENABLE           (HCI_HOST_BB | 0x001F)
#define HCI_WRITE_AUTH_ENABLE          (HCI_HOST_BB | 0x0020)
#define HCI_READ_ENC_MODE              (HCI_HOST_BB | 0x0021)
#define HCI_WRITE_ENC_MODE             (HCI_HOST_BB | 0x0022)
#define HCI_READ_CLASS_OF_DEVICE       (HCI_HOST_BB | 0x0023)
#define HCI_WRITE_CLASS_OF_DEVICE      (HCI_HOST_BB | 0x0024)
#define HCI_READ_VOICE_SETTING         (HCI_HOST_BB | 0x0025)
#define HCI_WRITE_VOICE_SETTING        (HCI_HOST_BB | 0x0026)
#define HCI_READ_AUTO_FLUSH_TIMEOUT    (HCI_HOST_BB | 0x0027)
#define HCI_WRITE_AUTO_FLUSH_TIMEOUT   (HCI_HOST_BB | 0x0028)
#define HCI_READ_NUM_BCAST_RETXS       (HCI_HOST_BB | 0x0029)
#define HCI_WRITE_NUM_BCAST_RETXS      (HCI_HOST_BB | 0x002A)
#define HCI_READ_HOLD_MODE_ACTIVITY    (HCI_HOST_BB | 0x002B)
#define HCI_WRITE_HOLD_MODE_ACTIVITY   (HCI_HOST_BB | 0x002C)
#define HCI_READ_TX_POWER_LEVEL        (HCI_HOST_BB | 0x002D)
#define HCI_READ_SCO_FLOW_CON_ENABLE   (HCI_HOST_BB | 0x002E)
#define HCI_WRITE_SCO_FLOW_CON_ENABLE  (HCI_HOST_BB | 0x002F)
#define HCI_SET_HCTOHOST_FLOW_CONTROL  (HCI_HOST_BB | 0x0031)
#define HCI_HOST_BUFFER_SIZE           (HCI_HOST_BB | 0x0033)
#define HCI_HOST_NUM_COMPLETED_PACKETS (HCI_HOST_BB | 0x0035)
#define HCI_READ_LINK_SUPERV_TIMEOUT   (HCI_HOST_BB | 0x0036)
#define HCI_WRITE_LINK_SUPERV_TIMEOUT  (HCI_HOST_BB | 0x0037)
#define HCI_READ_NUM_SUPPORTED_IAC     (HCI_HOST_BB | 0x0038)
#define HCI_READ_CURRENT_IAC_LAP       (HCI_HOST_BB | 0x0039)
#define HCI_WRITE_CURRENT_IAC_LAP      (HCI_HOST_BB | 0x003A)
#define HCI_READ_PAGESCAN_PERIOD_MODE  (HCI_HOST_BB | 0x003B)
#define HCI_WRITE_PAGESCAN_PERIOD_MODE (HCI_HOST_BB | 0x003C)
#define HCI_READ_PAGESCAN_MODE         (HCI_HOST_BB | 0x003D)
#define HCI_WRITE_PAGESCAN_MODE        (HCI_HOST_BB | 0x003E)
#define HCI_MAX_HOST_BB_OCF_V1_1       ( 0x003f)

/* 1.2 Features */
#define HCI_SET_AFH_CHANNEL_CLASS      (HCI_HOST_BB | 0x003F)
#define HCI_READ_INQUIRY_SCAN_TYPE     (HCI_HOST_BB | 0x0042)
#define HCI_WRITE_INQUIRY_SCAN_TYPE    (HCI_HOST_BB | 0x0043)
#define HCI_READ_INQUIRY_MODE          (HCI_HOST_BB | 0x0044)
#define HCI_WRITE_INQUIRY_MODE         (HCI_HOST_BB | 0x0045)
#define HCI_READ_PAGE_SCAN_TYPE        (HCI_HOST_BB | 0x0046)
#define HCI_WRITE_PAGE_SCAN_TYPE       (HCI_HOST_BB | 0x0047)
#define HCI_READ_AFH_CHANNEL_CLASS_M   (HCI_HOST_BB | 0x0048)
#define HCI_WRITE_AFH_CHANNEL_CLASS_M  (HCI_HOST_BB | 0x0049)
#define HCI_READ_ANON_MODE             (HCI_HOST_BB | 0x004A)
#define HCI_WRITE_ANON_MODE            (HCI_HOST_BB | 0x004B)
#define HCI_READ_ALIAS_AUTH_ENABLE     (HCI_HOST_BB | 0x004C)
#define HCI_WRITE_ALIAS_AUTH_ENABLE    (HCI_HOST_BB | 0x004D)
#define HCI_READ_ANON_ADDR_CHANGE_PARAMS  (HCI_HOST_BB | 0x004E)
#define HCI_WRITE_ANON_ADDR_CHANGE_PARAMS (HCI_HOST_BB | 0x004F)
#define HCI_RESET_FIXED_ADDRESS_ATTEMPTS_COUNTER  (HCI_HOST_BB | 0x0050)

/* 2.1 Features */
#define HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA   (HCI_HOST_BB | 0x0051)
#define HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA  (HCI_HOST_BB | 0x0052)
#define HCI_REFRESH_ENCRYPTION_KEY                (HCI_HOST_BB | 0x0053)
#define HCI_READ_SIMPLE_PAIRING_MODE              (HCI_HOST_BB | 0x0055)
#define HCI_WRITE_SIMPLE_PAIRING_MODE             (HCI_HOST_BB | 0x0056)
#define HCI_READ_LOCAL_OOB_DATA                   (HCI_HOST_BB | 0x0057)
#define HCI_READ_INQUIRY_RESPONSE_TX_POWER_LEVEL  (HCI_HOST_BB | 0x0058)
#define HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL    (HCI_HOST_BB | 0x0059)

#define HCI_ENHANCED_FLUSH             (HCI_HOST_BB | 0x005F)
#define HCI_SEND_KEYPRESS_NOTIFICATION (HCI_HOST_BB | 0x0060)

#define HCI_MAX_HOST_BB_OCF            ( 0x0061)


/******************************************************************************
   OP CODE defines - Informational Parameters
 *****************************************************************************/
#define HCI_READ_LOCAL_VER_INFO        (HCI_INFO | 0x0001)
/* 1.2 Feature */
#define HCI_READ_LOCAL_SUPP_COMMANDS   (HCI_INFO | 0x0002)
#define HCI_READ_LOCAL_SUPP_FEATURES   (HCI_INFO | 0x0003)
/* 1.2 Feature */
#define HCI_READ_LOCAL_EXT_FEATURES    (HCI_INFO | 0x0004)
#define HCI_READ_BUFFER_SIZE           (HCI_INFO | 0x0005)
#define HCI_READ_COUNTRY_CODE          (HCI_INFO | 0x0007)
#define HCI_READ_BD_ADDR               (HCI_INFO | 0x0009)
#define HCI_MAX_INFO_OCF_V1_1          ( 0x000A)
#define HCI_MAX_INFO_OCF               ( 0x000A)

/******************************************************************************
   OP CODE defines - Status Parameters
 *****************************************************************************/
#define HCI_READ_FAILED_CONTACT_COUNT  (HCI_STATUS | 0x0001)
#define HCI_RESET_FAILED_CONTACT_COUNT (HCI_STATUS | 0x0002)
#define HCI_GET_LINK_QUALITY           (HCI_STATUS | 0x0003)
#define HCI_READ_RSSI                  (HCI_STATUS | 0x0005)
#define HCI_MAX_STATUS_OCF_V1_1        ( 0x0006)

/* 1.2 Features */
#define HCI_READ_AFH_CHANNEL_MAP       (HCI_STATUS | 0x0006)
#define HCI_READ_CLOCK                 (HCI_STATUS | 0x0007)
#define HCI_MAX_STATUS_OCF             ( 0x0008)

/******************************************************************************
   OP CODE defines - Testing Parameters
 *****************************************************************************/
#define HCI_READ_LOOPBACK_MODE               (HCI_TEST | 0x0001)
#define HCI_WRITE_LOOPBACK_MODE              (HCI_TEST | 0x0002)
#define HCI_ENABLE_DUT_MODE                  (HCI_TEST | 0x0003)
#define HCI_MAX_TEST_OCF_V1_1                ( 0x0004)
#define HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE  (HCI_TEST | 0x0004)
#define HCI_MAX_TEST_OCF                     ( 0x0005)

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

enum {
    HCI_CHANNEL,
    HCI_EVENT,
};

typedef struct hci_command_t hci_command_t;
typedef hci_command_t   hci_read_buffer_size_t;
typedef struct hci_event_t hci_event_t;

struct hci_command_t {
    u16     op_code;
    u8      param_length;
    u8      params[0];
}__packed;

struct hci_event_t {
    u8      event_code;
    u8      param_length;
    u8      params[0];
}__packed;

typedef struct {
    hci_event_t ev;
    u8          num_hci_command_pkts;
    u16         op_code;
    u8          status;
    u8          params[0];
}__packed hci_ev_command_complete_t;

typedef struct {
    hci_event_t ev;
    u8          status;
    u8          num_hci_command_pkts;
    u16         op_code;
}__packed hci_ev_command_status_t;

drv_t *new_hci(void);
void delete_hci(drv_t *hci);

__END_DECLS
#endif
