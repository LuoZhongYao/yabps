/****************************************************************************
FILE
        rxslip.c  -  receive a slip packet

CONTAINS
        abcsp_uart_init -  initialise the slip reception code block
        abcsp_uart_deliverbytes  -  push received uart bytes into the library

        put_uart_byte  -  push received uart byte into slip decoder

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifdef RCS_STRINGS
static const char rxslip_c_id[]
  = "$Id: rxslip.c,v 1.1 2001/07/06 19:15:40 cjo Exp $";
#endif

#include "abcsp_private.h"

/* Slip decoder's state. */
enum rxstate_enum {
        rxstate_uninit,         /* Uninitialised. */
        rxstate_init_nosync,    /* First time unsynchronised. */
        rxstate_nosync,         /* No SLIP synchronisation. */
        rxstate_start,          /* Sending initial c0. */
        rxstate_body,           /* Sending message body. */
        rxstate_body_esc        /* Received escape character. */
        };
typedef enum rxstate_enum rxstate;

static rxstate state;

/* Forward reference. */
static bool put_uart_byte(uint8 c, bool *more);


/****************************************************************************
NAME
        abcsp_uart_init -  initialise the slip reception code block
*/

void abcsp_uart_init(void)
{
        state = rxstate_init_nosync;
}


/****************************************************************************
NAME
        abcsp_uart_deliverbytes  -  push received uart bytes into the library
*/

unsigned abcsp_uart_deliverbytes(char *buf, unsigned n)
{
        unsigned i;
        bool eom;

        /* Walk the UART byte buffer, pushing bytes into the rxbcsp engine
        until it either refuses a byte (no storage) or it signals that it
        has completed a higher-layer message (eom == TRUE). */

        for(i = 0, eom = FALSE; !eom && i < n; i++, buf++)
                if(! put_uart_byte((uint8)(*buf), &eom))
                        break;

        /* Push any partially completed received message (storage) back
        out to the surrounding code. */
        abcsp_rxmsg_flush();

        /* Report the number of bytes actually consumed from the buffer. */
        return(i);
}


/****************************************************************************
NAME
        put_uart_byte  -  push received uart byte into slip decoder

FUNCTION
        Pushes the UART byte "c" into the slip decoder and then through
        to the rxbcsp (bcsp message analyser) engine.

RETURNS
        TRUE if the decoder has consumed the byte "c", else FALSE.

        If the function returns TRUE then the value of "eom" (end of
        message) is set to TRUE if the consumed byte was the last of a
        slip message, else it sets it to FALSE.
*/

static
bool put_uart_byte(uint8 c, bool *eom)
{
        *eom = FALSE;

        switch(state) {
            default:
            case rxstate_uninit:
                ABCSP_EVENT(ABCSP_EVT_UNINITED);
                return(FALSE);
            case rxstate_init_nosync:
                if(c == 0xc0) {
                        ABCSP_EVENT(ABCSP_EVT_SLIP_SYNC);
                        state = rxstate_start;
                        }
                break;
            case rxstate_nosync:
                if(c == 0xc0)
                        state = rxstate_start;
                break;
            case rxstate_start:
                if(c == 0xc0)
                        break;
                state = rxstate_body;
                /* FALLTHROUGH. */
            case rxstate_body:
                if(c == 0xc0) {
                        /* Deliver message and signal "no more" to UART. */
                        abcsp_rxbcsp_done();
                        state = rxstate_nosync;
                        *eom = TRUE;
                        return(TRUE);
                        }
                else if(c == 0xdb) {
                        state = rxstate_body_esc;
                        }
                else {
                        if(!abcsp_rxbcsp_putbyte(c))
                                return(FALSE);
                        }
                break;
            case rxstate_body_esc:
                if(c == 0xdc) {
                        if(!abcsp_rxbcsp_putbyte((uint8)(0xc0)))
                                return(FALSE);
                        state = rxstate_body;
                        }
                else if(c == 0xdd) {
                        if(!abcsp_rxbcsp_putbyte((uint8)(0xdb)))
                                return(FALSE);
                        state = rxstate_body;
                        }
                else {
                        /* Byte sequence error.  Abandon current message. */
                        abcsp_rxbcsp_destroy();
                        ABCSP_EVENT(ABCSP_EVT_SLIP_SYNC_LOST);
                        state = rxstate_init_nosync;
                        }
                break;
            }
        
        /* Signal byte consumed and another byte wanted (eom == FALSE). */
        return(TRUE);
}
