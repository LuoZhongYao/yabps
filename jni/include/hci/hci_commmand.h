/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 10
 ************************************************/
#ifndef __HCI_COMMMAND_H__
#define __HCI_COMMMAND_H__

#include <zl/types.h>
__BEGIN_DECLS
#include <bd_addr.h>

/*******************************************************************
 * Defines for the OGF opcodes group
 ******************************************************************/
#define HCI_OGF_BIT_OFFSET          (10)

#define HCI_LINK                    (0x0400)
#define HCI_POLICY                  (0x0800)
#define HCI_HOST_BB                 (0x0c00)
#define HCI_INFO                    (0x1000)
#define HCI_STATUS                  (0x1400)
#define HCI_TEST                    (0x1800)
#define HCI_MAX_OGF                 (0x1c00)
#define HCI_MANUFACTURER_EXTENSION  (0xfc00)
#define HCI_OPCODE_GROUP_MASK       (0xfc00)
#define HCI_OPCODE_MASK             (0x03ff)

/****************************************************************
 *  Miscellaneous
 ****************************************************************/
#define HCI_NOP                     (0x0000)

/****************************************************************
 * OCF opcode defines - Link Control Commands
 ***************************************************************/
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
#define HCI_FLOW_SPEC		                   (HCI_POLICY | 0x0010)
#define HCI_SNIFF_SUB_RATE                     (HCI_POLICY | 0x0011)
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
#define HCI_WRITE_LOCAL_NAME           (HCI_HOST_BB | 0x0013)
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

/*-----------------------------------------------------------------------------*
 *
 *   Auto accept values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_AUTO_ACCEPT_OFF                 (0x01)
#define HCI_AUTO_ACCEPT_ON                  (0x02)
#define HCI_AUTO_ACCEPT_WITH_ROLE_SWITCH    (0x03)

/*-----------------------------------------------------------------------------*
 *
 *   Inquiry Values :   HCI_INQUIRY, HCI_PERIODIC_INQUIRY_MODE,
 *                      HCWI_WRITE_INQUIRYSCAN_ACTIVITY
 *
 *------------------------------------------------------------------------------*/
#define HCI_INQUIRY_LENGTH_MIN              (0x01)
#define HCI_INQUIRY_LENGTH_MAX              (0x30)
#define HCI_INQUIRY_RESPONSES_MIN           (0x01)
#define HCI_INQUIRY_RESPONSES_MAX           (0xFF)
#define HCI_INQUIRY_MAX_PERIOD_MIN          (0x0003)
#define HCI_INQUIRY_MAX_PERIOD_MAX          (0xFFFF)
#define HCI_INQUIRY_MIN_PERIOD_MIN          (0x0002)
#define HCI_INQUIRY_MIN_PERIOD_MAX          (0xFFFE)
#define HCI_INQUIRYSCAN_INTERVAL_MIN        (0x0012)
#define HCI_INQUIRYSCAN_INTERVAL_DEFAULT    (0x1000)
#define HCI_INQUIRYSCAN_INTERVAL_MAX        (0x1000)
#define HCI_INQUIRYSCAN_WINDOW_MIN          (0x0011)
#define HCI_INQUIRYSCAN_WINDOW_DEFAULT      (0x0012)
#define HCI_INQUIRYSCAN_WINDOW_MAX          (0x1000)

#ifndef HCI_MAX_INQUIRY_RESULTS
#define HCI_MAX_INQUIRY_RESULTS 1  /* Max responses in inquiry result message */
#endif

/*-----------------------------------------------------------------------------*
 *
 *   Scan enable values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_SCAN_ENABLE_OFF             (0x00) /* default */
#define HCI_SCAN_ENABLE_INQ             (0x01)
#define HCI_SCAN_ENABLE_PAGE            (0x02)
#define HCI_SCAN_ENABLE_INQ_AND_PAGE    (0x03)

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Page and Connection Accept Timer Defaults
 *
 *----------------------------------------------------------------------------*/
#define HCI_DEFAULT_PAGE_TIMEOUT                0x2000
#define HCI_DEFAULT_CONNECTION_ACCEPT_TIMEOUT   0x1FA0

/*-----------------------------------------------------------------------------*
 *
 *   HCI CREATE CONNECTION Boundary conditions
 *
 *------------------------------------------------------------------------------*/
#define HCI_DO_NOT_ALLOW_ROLE_SWITCH    (0x00)
#define HCI_ALLOW_ROLE_SWITCH           (0x01)

/*-----------------------------------------------------------------------------*
 *
 *  HCI ACCEPT CONNECTION REQUEST
 *  Role Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_ROLE_BECOME_MASTER          (0x00)
#define HCI_ROLE_STAY_SLAVE             (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   Authentication enable values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_AUTH_ENABLE_OFF             (0x00)
#define HCI_AUTH_ENABLE_ON              (0x01)    /* for all connections */

/*-----------------------------------------------------------------------------*
 *
 *   Encryption mode values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_ENC_OFF                     (0x00)
#define HCI_ENC_ON                      (0x01)

#define HCI_ENC_MODE_OFF                (0x00)
#define HCI_ENC_MODE_PT_TO_PT           (0x01)
#define HCI_ENC_MODE_PT_TO_PT_AND_BCAST (0x02)

/*-----------------------------------------------------------------------------*
 *
 *   Voice setting mask values
 *
 *------------------------------------------------------------------------------*/
#define HCI_VOICE_INPUT_MASK            (0x0300)
#define HCI_VOICE_INPUT_LIN             (0x0000)
#define HCI_VOICE_INPUT_MU_LAW          (0x0100)    
#define HCI_VOICE_INPUT_A_LAW           (0x0200)
#define HCI_VOICE_FORMAT_MASK           (0x00C0)
#define HCI_VOICE_FORMAT_1SCOMP         (0x0000)
#define HCI_VOICE_FORMAT_2SCOMP         (0x0040)
#define HCI_VOICE_FORMAT_SMAG           (0x0080)
#define HCI_VOICE_SAMP_SIZE_MASK        (0x0020)
#define HCI_VOICE_SAMP_SIZE_8BIT        (0x0000)
#define HCI_VOICE_SAMP_SIZE_16BIT       (0x0020)
#define HCI_VOICE_LINEAR_PCM_MASK       (0x001C)
#define HCI_VOICE_AIR_CODING_MASK       (0x0003)
#define HCI_VOICE_AIR_CODING_CVSD       (0x0000)
#define HCI_VOICE_AIR_CODING_MU_LAW     (0x0001)
#define HCI_VOICE_AIR_CODING_A_LAW      (0x0002)
#define HCI_VOICE_TRANSPARENT_DATA      (0x0003)

#define HCI_VOICE_SETTINGS_DEFAULT      (0x0060)
#define HCI_VOICE_SETTING_MAX_VALUE     (0x03FF) /* 10 bits meaningful */

/*-----------------------------------------------------------------------------*
 *
 *   Write Automatic Flush Timeout Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_MAX_FLUSH_TIMEOUT           (0x07FF)

/*-----------------------------------------------------------------------------*
 *
 *   Hold Mode Activity Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_HOLD_CURR_PWR_STATE         (0x00)
#define HCI_HOLD_SUSPEND_PAGE_SCAN      (0x01)
#define HCI_HOLD_SUSPEND_INQ_SCAN       (0x02)
#define HCI_HOLD_SUSPEND_PER_INQ        (0x04)

#define HCI_HOLD_MIN_PERIOD             (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   Sniff Mode Activity Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_SNIFF_MAX_INTERVAL_MIN      (0x01)
#define HCI_SNIFF_MIN_INTERVAL_MIN      (0x01)
#define HCI_SNIFF_ATTEMPT_MIN           (0x01)
#define HCI_SNIFF_ATTEMPT_MAX           (0x7FFF)
#define HCI_SNIFF_TIMEOUT_MIN           (0x00)
#define HCI_SNIFF_TIMEOUT_MAX           (0x7FFF)
#define HCI_SNIFF_SUB_RATE_LATENCY_MAX  (0xFFFE)
#define HCI_SNIFF_SUB_RATE_TIMEOUT_MAX  (0xFFFE)

/*-----------------------------------------------------------------------------*
 *
 *   Host Controller Channel Classification Mode Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_CHANNEL_CLASS_MODE_DISABLE  (0x00)
#define HCI_CHANNEL_CLASS_MODE_ENABLE   (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Version Information Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_VER_1_0               (0x00)
#define HCI_VER_1_1               (0x01)
#define HCI_VER_1_2               (0x02)
#define HCI_VER_2_0               (0x03)
#define HCI_VER_2_1               (0x04)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Country Code Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_CO_CODE_NA_AND_EUR     (0x00)
#define HCI_CO_CODE_FRANCE         (0x01)
#define HCI_CO_CODE_SPAIN          (0x02)
#define HCI_CO_CODE_JAPAN          (0x03)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Current device mode Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_BT_MODE_ACTIVE         (0x00)
#define HCI_BT_MODE_HOLD           (0x01)
#define HCI_BT_MODE_SNIFF          (0x02)
#define HCI_BT_MODE_PARK           (0x03)
#define HCI_BT_MODE_SCATTERMODE    (0x04)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Test Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_GEN_SELF_TEST          (0x00)
#define HCI_LOOPBACK_OFF           (0x00)
#define HCI_LOCAL_LOOPBACK         (0x01)
#define HCI_REMOTE_LOOPBACK        (0x02)
#define HCI_LOOPBACK_MODE_MAX      (0x03)

/*----------------------------------------------------------------------------*
 *
 *   HCI Link Type Values 
 *
 *---------------------------------------------------------------------------*/
#define HCI_LINK_TYPE_SCO          (0x00)
#define HCI_LINK_TYPE_ACL          (0x01)
#define HCI_LINK_TYPE_ESCO         (0x02)
#define HCI_LINK_TYPE_DONT_CARE    (0x03)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Page Scan Repetition Mode Values 
 *
 *------------------------------------------------------------------------------*/
#define HCI_PAGE_SCAN_REP_MODE_R0     (0x00)
#define HCI_PAGE_SCAN_REP_MODE_R1     (0x01)
#define HCI_PAGE_SCAN_REP_MODE_R2     (0x02)

/*----------------------------------------------------------------------------*
 *
 *   HCI Page Scan Mode Values 
 *
 *---------------------------------------------------------------------------*/
#define HCI_PAGE_SCAN_MODE_MANDATORY      (0x00)
#define HCI_PAGE_SCAN_MODE_OPTIONAL_1     (0x01)
#define HCI_PAGE_SCAN_MODE_OPTIONAL_2     (0x02)
#define HCI_PAGE_SCAN_MODE_OPTIONAL_3     (0x03)
#define HCI_PAGE_SCAN_MODE_DEFAULT        HCI_PAGE_SCAN_MODE_MANDATORY    

/*----------------------------------------------------------------------------*
 *
 *   HCI Page Scan Interval : HCI_WRITE_PAGESCAN_ACTIVITY 
 *
 *---------------------------------------------------------------------------*/
#define HCI_PAGESCAN_INTERVAL_MIN       ( 0x12)
#define HCI_PAGESCAN_INTERVAL_DEFAULT   ( 0x800)
#define HCI_PAGESCAN_INTERVAL_MAX       ( 0x1000)

/*----------------------------------------------------------------------------*
 *
 *   HCI Page Scan Window : HCI_WRITE_PAGESCAN_ACTIVITY 
 *
 *---------------------------------------------------------------------------*/
#define HCI_PAGESCAN_WINDOW_MIN       ( 0x11)
#define HCI_PAGESCAN_WINDOW_DEFAULT   ( 0x12)
#define HCI_PAGESCAN_WINDOW_MAX       ( 0x1000)

/*----------------------------------------------------------------------------*
 *
 *   HCI Page Scan PERIOD : HCI_WRITE_PAGESCAN_PERIOD_MODE 
 *
 *---------------------------------------------------------------------------*/
#define HCI_PAGESCAN_PERIOD_MODE_P0         ( 0x00)
#define HCI_PAGESCAN_PERIOD_MODE_P1         ( 0x01)
#define HCI_PAGESCAN_PERIOD_MODE_P2         ( 0x02)
#define HCI_PAGESCAN_PERIOD_MODE_DEFAULT    HCI_PAGESCAN_PERIOD_MODE_P0

/*----------------------------------------------------------------------------*
 *
 *   HCI Page and Inquiry Scan Type : HCI_WRITE_PAGE_SCAN_TYPE 
 *                                    HCI_WRITE_INQUIRY_SCAN_TYPE
 *
 *---------------------------------------------------------------------------*/
#define HCI_SCAN_TYPE_LEGACY           ( 0x00)
#define HCI_SCAN_TYPE_INTERLACED       ( 0x01)

/*----------------------------------------------------------------------------*
 *
 *   HCI Inquiry Mode : HCI_WRITE_INQUIRY_MODE 
 *
 *---------------------------------------------------------------------------*/
#define HCI_INQUIRY_MODE_STANDARD      ( 0x00)
#define HCI_INQUIRY_MODE_WITH_RSSI     ( 0x01)
#define HCI_INQUIRY_MODE_WITH_EIR      ( 0x02)

/*----------------------------------------------------------------------------*
 *
 *   HCI Clock offset Values 
 *
 *---------------------------------------------------------------------------*/
#define HCI_CLOCK_OFFSET_MASK       (0x7fff)
#define HCI_CLOCK_OFFSET_VALID_MASK (0x8000)
#define HCI_CLOCK_OFFSET_INVALID    (0x0000)
#define HCI_CLOCK_OFFSET_VALID      (0x8000)

/*----------------------------------------------------------------------------*
 *
 *   HCI Link Policy Settings
 *
 *---------------------------------------------------------------------------*/
#define DISABLE_ALL_LM_MODES         (0x0000)
#define ENABLE_MS_SWITCH             (0x0001)
#define ENABLE_HOLD                  (0x0002)
#define ENABLE_SNIFF                 (0x0004)
#define ENABLE_PARK                  (0x0008)
#define ENABLE_SCATTER_MODE          (0x0010)

/*----------------------------------------------------------------------------*
 *
 *   HCI Filter types
 *
 *---------------------------------------------------------------------------*/
#define CLEAR_ALL_FILTERS            (0x00)
#define INQUIRY_RESULT_FILTER        (0x01)
#define CONNECTION_FILTER            (0x02)

/*----------------------------------------------------------------------------*
 *
 *   HCI Filter condition types
 *
 *---------------------------------------------------------------------------*/
#define NEW_DEVICE_RESPONDED         (0x00)
#define ALL_CONNECTION               (0x00)
#define CLASS_OF_DEVICE_RESPONDED    (0x01)
#define ADDRESSED_DEVICE_RESPONDED   (0x02)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Pin types
 *
 *------------------------------------------------------------------------------*/
#define HCI_VARIABLE_PIN    (0x00)
#define HCI_FIXED_PIN       (0x01)


/*-----------------------------------------------------------------------------*
 *
 *   Enhanced flushable packet types
 *   NB values 0x01 - 0xFF Reserved for future use
 *------------------------------------------------------------------------------*/
#define HCI_AUTO_FLUSHABLE_ONLY (0x00)
#define FLUSH_INFINITE_TIMEOUT 0x0


/*-----------------------------------------------------------------------------*
 *
 *   HCI Pin Code Length
 *
 *------------------------------------------------------------------------------*/
#define HCI_MIN_PIN_LENGTH  (0x01)
#define HCI_MAX_PIN_LENGTH  (0x10)

/******************************************************************************
 *
 *  HCI Local Name Length
 *
 *****************************************************************************/
#define HCI_MAX_LOCAL_NAME_LENGTH 248

/*-----------------------------------------------------------------------------*
 *
 *   Size of link keys
 *
 *------------------------------------------------------------------------------*/
#define SIZE_LINK_KEY   0x10

/*---------------------------------------------------------------------------*
 *
 *  Size of Out Of Band data hash and rand
 *
 * --------------------------------------------------------------------------*/

#define SIZE_OOB_DATA   0x10

/*-----------------------------------------------------------------------------*
 *
 *   HCI Read stored link key read all flag types
 *
 *------------------------------------------------------------------------------*/
#define RETURN_BDADDR   (0x00)
#define RETURN_ALL      (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Delete stored link key read all flag types
 *
 *------------------------------------------------------------------------------*/
#define DELETE_BDADDR   (0x00)
#define DELETE_ALL      (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   HCI IAC LAP Boundary values and Other Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_NUM_CURRENT_IAC_MIN   (0x01)
#define HCI_NUM_CURRENT_IAC_MAX   (0x40)
#define HCI_IAC_LAP_MIN           (0x9E8B00)
#define HCI_IAC_LAP_MAX           (0x9E8B3F)

#define HCI_INQ_CODE_GIAC  (0x9e8b33)
/*-----------------------------------------------------------------------------*
 *
 *   HCI Connection Accept Timeout values
 *
 *------------------------------------------------------------------------------*/
#define HCI_CONNECTION_ACCEPT_TIMEOUT_MIN   (0x01)
#define HCI_CONNECTION_ACCEPT_TIMEOUT_MAX   (0x0B540)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Handle Range
 *
 *------------------------------------------------------------------------------*/
#define HCI_HANDLE_MAX   (0x0EFF)
#define HCI_HANDLE_INVALID ( 0xffff)

/*----------------------------------------------------------------------------*
 *
 *   HCI Link Supervision Timeout Values
 *
 *---------------------------------------------------------------------------*/
#define HCI_LINK_SUPERVISION_INFINITY   (0x0000)
#define HCI_LINK_SUPERVISION_MIN        (0x0001)
#define HCI_LINK_SUPERVISION_DEFAULT    (0x7D00)
#define HCI_LINK_SUPERVISION_MAX        (0xFFFF)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Transmit Power Type Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_READ_CURRENT_TX_POWER       (0x00)
#define HCI_READ_MAX_TX_POWER           (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   HCI SCO Flow Control Type Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_SCO_FLOW_CONTROL_DISABLED   (0x00)
#define HCI_SCO_FLOW_CONTROL_ENABLED    (0x01)

/*-----------------------------------------------------------------------------*
 *
 *   HCI HC to H Flow Control Type Values
 *
 *------------------------------------------------------------------------------*/
#define HCI_HCITOH_FLOW_CONTROL_DISABLED            (0x00)
#define HCI_HCITOH_FLOW_CONTROL_ENABLED_ACL_ONLY    (0x01)
#define HCI_HCITOH_FLOW_CONTROL_ENABLED_SCO_ONLY    (0x02)
#define HCI_HCITOH_FLOW_CONTROL_ENABLED_ACL_AND_SCO (0x03)

/*-----------------------------------------------------------------------------*
 *
 *   HCI Write Stored Link Key, HCI Return Link Keys Event, 
 *   maximum number of keys that can be present
 *   in a write stored link key. Max command parameter size= 255 bytes.
 *      Num keys parameter = 1 byte
 *      (BD_ADDR + Key) = (6+16) = X
 *      Max Keys = (255-1)/X = 10
 *
 *------------------------------------------------------------------------------*/
#define HCI_STORED_LINK_KEY_MAX             (10)
#define HCI_RETURN_LINK_KEY_VAR_ARG_SIZE    22

/*-----------------------------------------------------------------------------*
 *
 *   HCI Link Key Notification Event Key Type
 *
 *------------------------------------------------------------------------------*/
#define HCI_COMBINATION_KEY                 (0)
#define HCI_LOCAL_UNIT_KEY                  (1)
#define HCI_REMOTE_UNIT_KEY                 (2)
#define HCI_DEBUG_COMBINATION_KEY           (3)
#define HCI_UNAUTHENTICATED_COMBINATION_KEY (4)
#define HCI_AUTHENTICATED_COMBINATION_KEY   (5)
#define HCI_CHANGED_COMBINATION_KEY         (6)
#define HCI_KEY_TYPE_UNKNOWN                (0xFF)


#define __COMMAND u16 op_code;u8 length

typedef struct {
    __COMMAND;
    u8 data[0];
} __packed hci_command_t;

typedef struct {
    __COMMAND;
    u16 handle; 
    bd_addr_t bd_addr;
} __packed hci_with_ch_command_t;

typedef struct {
    __COMMAND;
    bd_addr_t bd_addr;
} __packed hci_with_bd_command_t;

typedef struct {
    u16 handle_plus_flags;
    u16 length;
    u8  data[0];
} __packed hci_acl_data_command_t;

typedef struct {
    u16 handle;
    u8  length;
    u8  data[0];
} __packed hci_sco_data_command_t;

typedef struct {
    __COMMAND;
    u32 lap:24;
    u32 inquiry_length:8;
    u8  responses;
} __packed hci_inquiry_t;

typedef struct {
    __COMMAND;
} __packed hci_inquiry_cancel_t;

typedef struct {
    __COMMAND;
    bd_addr_t bd_addr;
    u8 role;
} __packed hci_accept_connection_req_t;

typedef struct {
    __COMMAND;
    u16 max_period_length;
    u16 min_period_length;
    u32 lap:24;
    u32 inquiry_length:8;
    u8  responses;
} __packed hci_periodic_inquiry_mode_t;

typedef struct {
    __COMMAND;
} __packed hci_exit_periodic_inquiry_mode_t;

typedef struct {
    __COMMAND;
    bd_addr_t bd_addr;
    u16 ptk_type;
    u8  page_scan_rep_mode;
    u8  page_scan_mode;
    u16 clock_offset;
    u8  allow_role_switch;

} __packed hci_create_connection_t;

typedef struct {
    __COMMAND;
    u16 handle;
    u8  resson;
} __packed hci_disconnect_t;

typedef struct {
    __COMMAND;
    u16 handle;
    u16 ptk_type;
    bd_addr_t bd_addr;
} __packed hci_add_sco_connection_t;

typedef struct {
    __COMMAND;
    bd_addr_t bd_addr;
} __packed hci_create_connection_cancel_t;

typedef struct {
    __COMMAND;
    bd_addr_t bd_addr;
    u8  reason;
} __packed hci_reject_connection_req_t;

typedef struct {
	__COMMAND;
	bd_addr_t bd_addr;
    u8 value[SIZE_LINK_KEY];
} __packed hci_link_key_req_replay_t;

typedef struct {
	__COMMAND;
	bd_addr_t bd_addr;
} __packed hci_link_key_req_reply_ret_t;

typedef struct {
	__COMMAND;
	bd_addr_t bd_addr;
    u8 pin_length;
    u8 pin[HCI_MAX_PIN_LENGTH];
} __packed hci_pin_code_req_reply_t;

typedef struct {
	__COMMAND;
	bd_addr_t bd_addr;
} __packed hci_pin_code_req_neg_reply_t;

typedef struct {
	__COMMAND;
	u16 handle;u16 pkt_type;
} __packed hci_change_conn_pkt_type_t;

typedef struct {
	__COMMAND;
	u16 handle;
    bd_addr_t bd_addr;
} __packed hci_auth_req_t;

typedef struct {
	__COMMAND;
	u16 handle;
    bd_addr_t bd_addr;
    u8 enc_enable;
} __packed hci_set_conn_encryption_t;

typedef struct {
    __COMMAND;
    u16 acl_packet_length;
    u8  sco_packet_length;
    u16 acl_total_packets;
    u16 sco_total_packets;
}__packed hci_host_buffer_size_t;

typedef struct {
    __COMMAND;
    u8 scan_enable;
}__packed hci_write_scan_enable_t ;

typedef struct {
    __COMMAND;
}__packed hci_read_local_version_t;

typedef struct {
    __COMMAND;
}__packed hci_read_local_features_t;

typedef struct {
    __COMMAND;
}__packed hci_read_buffer_size_t;

typedef struct {
    __COMMAND;
    u8 name[HCI_MAX_LOCAL_NAME_LENGTH];
}__packed hci_write_local_name_t;

typedef struct {
    __COMMAND;
}__packed hci_read_local_name_t;

void hci_inquiry(u32 lap,u8 inquiry_length,u8 num_responses);
void hci_accept_connection(bd_addr_t *bd_addr,u8 role);
void hci_pin_code_req_reply(bd_addr_t *bd_addr,u8 pin_length,u8 *pin);
void hci_write_scan_enable(u8 enable);
void hci_read_local_version(void);
void hci_read_buffer_size(void);
void hci_host_buffer_size(u16 acl_packet_length,u8 sco_packet_length,
        u16 acl_total_packets,u16 sco_total_packets);
void hci_read_local_features(void);
void hci_write_local_name(const u8 *name,const u16 len);
void hci_read_local_name(void);



__END_DECLS
#endif

