/****************************************************************************
FILE
        rxslip.h  -  read slip messages from the uart

CONTAINS
        abcsp_uart_init -  initialise the slip reception code block
        abcsp_uart_deliverbytes  -  push received uart bytes into the library

DESCRIPTION
        This implements the receive (from UART) side of the "SLIP Layer",
        described in CSR document bc01-s-006 (aka AN004).

        The basic task of this code is to translate a train of bytes
        received from the UART into BCSP message frames.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __RXSLIP_H__
#define __RXSLIP_H__
 
#ifdef RCS_STRINGS
static const char rxslip_h_id[]
  = "$Id: rxslip.h,v 1.1 2001/07/06 19:15:40 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_uart_init -  initialise the slip reception code block

FUNCTION
        Initialises the state of the rxslip code block.

        This must be called before all other functions described in this
        file.

        This may be called at any time to reinitialise the state of the
        code block.
*/

extern void abcsp_uart_init(void);


/****************************************************************************
NAME
        abcsp_uart_deliverbytes  -  push received uart bytes into the library
*/

/* Described and declared in abcsp.h. */


#endif /* __RXSLIP_H__ */
