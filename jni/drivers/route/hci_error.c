#include <hci/hci_error.h>
#include <zl/util.h>
static const char *errors [] = {
    "Success",
    "Unknown HCI Command",
    "Unknown Connection Identifier",
    "Hardware Failure",
    "Page Timeout",
    "Authentication Failure",
    "PIN or Key Missing",
    "Memory Capacity Exceeded",
    "Connection Timeout",
    "Connection Limit Exceeded",
    "Synchronous Connection Limit To A Device Exceeded",
    "ACL Connection Already Exists",
    "Command Disallowed",
    "Connection Rejected due to Limited Resources",
    "Connection Rejected Due To Security Reasons",
    "Connection Rejected due to Unacceptable BD_ADDR",
    "Connection Accept Timeout Exceeded",
    "Unsupported Feature or Parameter Value",
    "Invalid HCI Command Parameters",
    "Remote User Terminated Connection",
    "Remote Device Terminated Connection due to Low Resources",
    "Remote Device Terminated Connection due to Power Off",
    "Connection Terminated By Local Host",
    "Repeated Attempts",
    "Pairing Not Allowed",
    "Unknown LMP PDU",
    "Unsupported Remote Feature / Unsupported LMP Feature",
    "SCO Offset Rejected",
};

const char *hci_error_string(u8 err)
{
    if(err < ARRAY_SIZE(errors))
        return errors[err];
    return "Invalid error code";
}
