/****************************************************************************
FILE
        txslip.h  -  convert messages to slip and send them to the uart

CONTAINS
        abcsp_txslip_init  -  initialise the slip encoder
        abcsp_txslip_msgdelim  -  send slip fame message delimiter to uart
        abcsp_txslip_sendbuf -  slip encode buffer and send to uart
        abcsp_txslip_flush  -  send buffer to uart
        abcsp_txslip_destroy  -  discard any message currently being slipped

DESCRIPTION
        The code block described by this file implements the transmit
        (to UART) side of the "SLIP Layer", described in CSR document
        bc01-s-006 (aka AN004).

        Functions abcsp_txslip_msgdelim() and abcsp_txslip_sendbuf()
        may obtain a buffer from the UART driver and may write bytes to
        the buffer, but they do not pass the buffer back to the UART;
        this action is performed by abcsp_txslip_flush().  This slightly
        odd structure facilitates the constraint that one call to
        abcsp_pumptxmsgs() should send, at most, one buffer to the UART
        driver.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __TXSLIP_H__
#define __TXSLIP_H__
 
#ifdef RCS_STRINGS
static const char txslip_h_id[]
  = "$Id: txslip.h,v 1.1 2001/07/06 19:15:44 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_txslip_init  -  initialise the slip encoder

FUNCTION
        Initialises the state of the txslip code block.

        This must be called before all other functions described in this
        file.

        This may be called at any time to reinitialise the state of the
        code block.
*/

extern void abcsp_txslip_init( void );


/****************************************************************************
NAME
        abcsp_txslip_msgdelim  -  send slip fame message delimiter to uart

RETURNS
	TRUE if the function has passed a BCSP frame delimiter byte (0xc0) to
	the UART buffer, else FALSE.
*/

extern bool abcsp_txslip_msgdelim(void);

/* The delimiter is used to mark the start and end of each frame. */
#define abcsp_txslip_msgstart()          abcsp_txslip_msgdelim()
#define abcsp_txslip_msgend()            abcsp_txslip_msgdelim()


/****************************************************************************
NAME
        abcsp_txslip_sendbuf -  slip encode buffer and send to uart

FUNCTION
        Slip-encodes and sends up to bufsiz bytes from "buf" to the UART.

RETURNS
        The number of bytes consumed from "buf".

	If the code wishes to be given further bytes to translate then *more
	is set to TRUE on return, else it is set to FALSE.

NOTE
        Code under this function makes at most one call to
        ABCSP_UART_SENDBYTES().
*/

extern uint16 abcsp_txslip_sendbuf(uint8 *buf, uint16 bufsiz, bool *more);


/****************************************************************************
NAME
        abcsp_txslip_flush  -  send buffer to uart

FUNCTION
        If the slip-encoder is holding a buffer obtained via a call to
        ABCSP_UART_GETBUF() then this is released by making a call to
        ABCSP_UART_SENDBYTES().
*/

extern void abcsp_txslip_flush(void);


/****************************************************************************
NAME
        abcsp_txslip_destroy  -  discard any message currently being slipped

FUNCTION
        Releases any resources currently held by the slip-encoder and
        reinitalises it.
*/

extern void abcsp_txslip_destroy(void);


#endif  /* __TXSLIP_H__ */
