#ifndef __PBTEST_H__
#define __PBTEST_H__

/****************************************************************************

   				? Cambridge Silicon Radio Ltd, 2004
 	 			
				All rights reserved

FILE:			abcsp_intf_hl.h 

DESCRIPTION:	Common header for the abcsp interface.  The abcsp code itself
				does not #include this file.

REVISION:		$Revision: #1 $ 

****************************************************************************/

#include <string.h>
#include <hci.h>
#include "abcsp.h" 
#include "abcsp_events.h" 
#include "abcsp_panics.h" 
#include "config_le.h" 
#include "config_event.h" 
#include "config_rxmsg.h" 
#include "config_timer.h" 
#include "config_txmsg.h" 
#include "config_panic.h" 

#ifdef __cplusplus
extern "C" {
#endif

/* This instance of the program. */

unsigned char uart_tx_buffer_empty(void);
void abcsp_event(unsigned e);
void abcsp_req_pumptxmsgs(void);
void abcsp_panic(unsigned e);
ABCSP_RXMSG *abcsp_rxmsg_create(unsigned len);
char *abcsp_rxmsg_getbuf(ABCSP_RXMSG *m, unsigned *n);
void abcsp_rxmsg_write(ABCSP_RXMSG *m, char *buf, unsigned n);
void abcsp_rxmsg_complete(ABCSP_RXMSG *m);
void abcsp_cancel_bcsp_timer(void);
void abcsp_cancel_tshy_timer(void);
void abcsp_cancel_tconf_timer(void);
void abcsp_start_bcsp_timer(void);
void abcsp_start_tshy_timer(void);
void abcsp_start_tconf_timer(void);
void abcsp_txmsg_init_read(ABCSP_TXMSG *msg);
unsigned abcsp_txmsg_length(ABCSP_TXMSG *msg);
char *abcsp_txmsg_getbuf(ABCSP_TXMSG *msg, unsigned *buflen);
void abcsp_txmsg_taken(ABCSP_TXMSG *msg, unsigned ntaken);
void abcsp_txmsg_done(ABCSP_TXMSG *msg);
void abcsp_uart_sendbytes(char *buf, unsigned n);
void abcsp_txmsg_done(ABCSP_TXMSG *msg);
void abcsp_pumptxmsgsOut(void);			/*Called by bg_int to do pump*/
void AbcspRxMmsgDestroy(ABCSP_RXMSG *m);

#ifdef ROM_BUILD_ENABLE
void abcsp_restart(void);
#endif


#ifdef __cplusplus
}
#endif 

#endif  /* __PBTEST_H__ */
