/****************************************************************************
FILE
        txslip.c  -  send slip packets

CONTAINS
        abcsp_txslip_init  -  initialise the slip transmit block
        abcsp_txslip_msgdelim  -  send slip fame message delimiter to uart
        abcsp_txslip_sendbuf -  slip encode buffer and send to uart
        abcsp_txslip_flush  -  send buffer to uart
        abcsp_txslip_destroy  -  discard any message currently being slipped

        put_uart_byte  -  send byte to uart buffer

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifdef RCS_STRINGS
static const char txslip_c_id[]
  = "$Id: txslip.c,v 1.1 2001/07/06 19:15:44 cjo Exp $";
#endif

#include "abcsp_private.h"
#include "config_txmsg.h"


/* Slip state machine states. */
enum txstate_enum {
        txstate_normal,         /* Sending unescaped characters. */
        txstate_esc_c0,         /* Sending 0xdc of {0xdb, 0xdc} for 0xc0. */
        txstate_esc_db          /* Sending 0xdd of {0xdb, 0xdd} for 0xdb. */
        };
typedef enum txstate_enum txstate;
static txstate state;


/* Forward reference. */
static bool put_uart_byte(uint8 c);


/****************************************************************************
NAME
        abcsp_txslip_init  -  initialise the slip encoder
*/

void abcsp_txslip_init(void)
{
        state = txstate_normal;
}


/****************************************************************************
NAME
        abcsp_txslip_msgdelim  -  send slip fame message delimiter to uart
*/

bool abcsp_txslip_msgdelim(void)
{
        return(put_uart_byte(0xc0));
}


/****************************************************************************
NAME
        abcsp_txslip_sendbuf -  slip encode buffer and send to uart
*/

uint16 abcsp_txslip_sendbuf(uint8 *buf, uint16 bufsiz, bool *more)
{
        uint16 nsent;
        uint8 c;

        for(nsent = 0, *more = FALSE ; nsent < bufsiz ; ) {
                switch(state) {
                    case txstate_normal:
                        switch(c = *buf) {
                            case 0xc0:
                                if(!put_uart_byte(0xdb))
                                        return(nsent);
                                state = txstate_esc_c0;
                                break;
                            case 0xdb:
                                if(!put_uart_byte(0xdb))
                                        return(nsent);
                                state = txstate_esc_db;
                                break;
                            default:
                                if(!put_uart_byte(c))
                                        return(nsent);
                                ++buf;
                                ++nsent;
                                break;
                            }
#ifdef  ABCSP_TXCRC
                        if(abcsp_txrx.txcrc)
                                abcsp_crc_update(&abcsp_txmsg_crc, c);
#endif  /* ABCSP_TXCRC */  
                        break;
                    case txstate_esc_c0:
                        if(!put_uart_byte(0xdc))
                                return(nsent);
                        ++buf;
                        ++nsent;
                        state = txstate_normal;
                        break;
                    case txstate_esc_db:
                        if(!put_uart_byte(0xdd))
                                return(nsent);
                        ++buf;
                        ++nsent;
                        state = txstate_normal;
                        break;
                    }
                }

        *more = TRUE;
        return(nsent);
}


/****************************************************************************
NAME
        put_uart_byte  -  send byte to uart buffer
*/

static char *ubuf;
static unsigned ubufsiz;
static unsigned ubufindex;

static
bool put_uart_byte(uint8 c)
{
        if(ubuf == (char*)(NULL)) {
                if((ubuf = ABCSP_UART_GETTXBUF(&ubufsiz)) == (char*)(NULL))
                        return(FALSE);
                ubufindex = 0;
                if(ubufsiz == 0)
                        return(FALSE);
                }

        if(ubufindex >= ubufsiz)
                return(FALSE);

        ubuf[ubufindex++] = (char)(c);
        return(TRUE);
} 


/****************************************************************************
NAME
        abcsp_txslip_flush  -  send buffer to uart
*/

void abcsp_txslip_flush(void)
{
        if(ubuf != (char*)(NULL)) {
                ABCSP_UART_SENDBYTES(ubuf, ubufindex);
                ubuf = (char*)(NULL);
                }
}


/****************************************************************************
NAME
        abcsp_txslip_destroy  -  discard any message currently being slipped
*/

/* Not currently used.
void abcsp_txslip_destroy(void)
{
        abcsp_txslip_flush();
        abcsp_txslip_init();
} */
