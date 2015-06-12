/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2015 Jun 10
 ************************************************/
#ifndef __HCI_ERROR_H__
#define __HCI_ERROR_H__

#include <zl/types.h>

/******************************************************************************
 *       HCI Successful return value 
 ******************************************************************************/
#define HCI_SUCCESS                         (0x00)
#define HCI_COMMAND_CURRENTLY_PENDING       (0x00)

/******************************************************************************
    HCI Error codes 
 *****************************************************************************/
#define HCI_ERROR_ILLEGAL_COMMAND           (0x01)
#define HCI_ERROR_NO_CONNECTION             (0x02)
#define HCI_ERROR_HARDWARE_FAIL             (0x03)
#define HCI_ERROR_PAGE_TIMEOUT              (0x04)
#define HCI_ERROR_AUTH_FAIL                 (0x05)
#define HCI_ERROR_KEY_MISSING               (0x06)
#define HCI_ERROR_MEMORY_FULL               (0x07)
#define HCI_ERROR_CONN_TIMEOUT              (0x08)
#define HCI_ERROR_MAX_NR_OF_CONNS           (0x09)
#define HCI_ERROR_MAX_NR_OF_SCO             (0x0A)
#define HCI_ERROR_MAX_NR_OF_ACL             (0x0B)
#define HCI_ERROR_COMMAND_DISALLOWED        (0x0C)
#define HCI_ERROR_REJ_BY_REMOTE_NO_RES      (0x0D)  /* no resource */
#define HCI_ERROR_REJ_BY_REMOTE_SEC         (0x0E)  /* security violation */
#define HCI_ERROR_REJ_BY_REMOTE_PERS        (0x0F)  /* personal device */
#define HCI_ERROR_HOST_TIMEOUT              (0x10) 
#define HCI_ERROR_UNSUPPORTED_FEATURE       (0x11)  /* or incorrect param value */
#define HCI_ERROR_ILLEGAL_FORMAT            (0x12) 
#define HCI_ERROR_OETC_USER                 (0x13)  /* other end terminated */
#define HCI_ERROR_OETC_LOW_RESOURCE         (0x14)  /* other end terminated */
#define HCI_ERROR_OETC_POWERING_OFF         (0x15)  /* other end terminated */
#define HCI_ERROR_CONN_TERM_LOCAL_HOST      (0x16)  /* local host terminated */
#define HCI_ERROR_AUTH_REPEATED             (0x17)  
#define HCI_ERROR_PAIRING_NOT_ALLOWED       (0x18) 
#define HCI_ERROR_UNKNOWN_LMP_PDU           (0x19) 
#define HCI_ERROR_UNSUPPORTED_REM_FEATURE   (0x1A) /* HCI_ERROR_UNSUPPORTED_LMP_FEATURE */
#define HCI_ERROR_SCO_OFFSET_REJECTED       (0x1B) 
#define HCI_ERROR_SCO_INTERVAL_REJECTED     (0x1C) 
#define HCI_ERROR_SCO_AIR_MODE_REJECTED     (0x1D) 
#define HCI_ERROR_INVALID_LMP_PARAMETERS    (0x1E) 
#define HCI_ERROR_UNSPECIFIED               (0x1F) 
#define HCI_ERROR_UNSUPP_LMP_PARAM          (0x20) 
#define HCI_ERROR_ROLE_CHANGE_NOT_ALLOWED   (0x21) 
#define HCI_ERROR_LMP_RESPONSE_TIMEOUT      (0x22)
#define HCI_ERROR_LMP_TRANSACTION_COLLISION (0x23)
#define HCI_ERROR_LMP_PDU_NOT_ALLOWED       (0x24) 
#define HCI_ERROR_ENC_MODE_NOT_ACCEPTABLE   (0x25) 
#define HCI_ERROR_UNIT_KEY_USED             (0x26) 
#define HCI_ERROR_QOS_NOT_SUPPORTED         (0x27) 
#define HCI_ERROR_INSTANT_PASSED            (0x28) 
#define HCI_ERROR_PAIR_UNIT_KEY_NO_SUPPORT  (0x29)
#define HCI_ERROR_DIFFERENT_TRANSACTION_COLLISION  (0x2A)
#define HCI_ERROR_SCM_INSUFFICIENT_RESOURCES (0x2B)
#define HCI_ERROR_QOS_UNACCEPTABLE_PARAMETER (0x2C)
#define HCI_ERROR_QOS_REJECTED              (0x2D)
#define HCI_ERROR_CHANNEL_CLASS_NO_SUPPORT  (0x2E)
#define HCI_ERROR_INSUFFICIENT_SECURITY     (0x2F)
#define HCI_ERROR_PARAM_OUT_OF_MAND_RANGE   (0x30)
#define HCI_ERROR_SCM_NO_LONGER_REQD        (0x31)
#define HCI_ERROR_ROLE_SWITCH_PENDING       (0x32)
#define HCI_ERROR_SCM_PARAM_CHANGE_PENDING  (0x33)
#define HCI_ERROR_RESVD_SLOT_VIOLATION      (0x34)
#define HCI_ERROR_ROLE_SWITCH_FAILED        (0x35)
#define HCI_ERROR_INQUIRY_RESPONSE_DATA_TOO_LARGE (0x36)
#define HCI_ERROR_SP_NOT_SUPPORTED_BY_HOST  (0x37)
#define HCI_ERROR_HOST_BUSY_PAIRING         (0x38)

const char *hci_error_string(u8 err);
#endif

