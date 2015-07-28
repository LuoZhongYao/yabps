/****************************************************************************
FILE
        abcsp_test.h  -  mappings to wire abcsp to test environment

CONTAINS

DESCRIPTION
	This file holds rough test code mappings, to be used where ABCSP_TEST
	is #defined.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __ABCSP_TEST_H__
#define __ABCSP_TEST_H__
 
#include <zl/log.h>
#ifdef RCS_STRINGS
static const char abcsp_test_h_id[]
  = "$Id: abcsp_test.h,v 1.1 2001/07/06 20:11:03 cjo Exp $";
#endif

#ifdef __LOG_ABCSP__
#define ABCSP_LOGD(fmt,...) ABCSP_LOGD("[BSP] "fmt,##__VA_ARGS__)
#else
#define ABCSP_LOGD(...)
#endif
#define ABCSP_LOGE(fmt,...) LOGE("[BSP] "fmt,##__VA_ARGS__)

#ifdef ABCSP_TEST

#include "abcsp.h"
#include "config_event.h"
#include "config_panic.h"
#include "config_malloc.h"
#include "config_rxmsg.h"
#include "config_timer.h"
#include "config_txmsg.h"
#include "config_le.h"


#define test_abcsp_event(n) ABCSP_LOGD(#n)
//extern void test_abcsp_event(unsigned e);
#define test_abcsp_req_pumptxmsgs() ABCSP_LOGD("Req pumptxmsgs")
//extern void test_abcsp_req_pumptxmsgs(void);
#define test_abcsp_panic(e) do{ABCSP_LOGE(#e);exit(1);}while(0)
//extern void test_abcsp_panic(unsigned e);
extern ABCSP_RXMSG *test_abcsp_rxmsg_create(unsigned len);
extern char *test_abcsp_rxmsg_getbuf(ABCSP_RXMSG *m, unsigned *n);
extern void test_abcsp_rxmsg_write(ABCSP_RXMSG *m, char *buf, unsigned n);
extern void test_abcsp_rxmsg_complete(ABCSP_RXMSG *m);
extern void test_abcsp_delivermsg(ABCSP_RXMSG *m, unsigned chan, unsigned rel);
extern void test_abcsp_rxmsg_destroy(ABCSP_RXMSG *m);
extern void test_abcsp_cancel_bcsp_timer(void);
extern void test_abcsp_cancel_tshy_timer(void);
extern void test_abcsp_cancel_tconf_timer(void);
extern void test_abcsp_start_bcsp_timer(void);
extern void test_abcsp_start_tshy_timer(void);
extern void test_abcsp_start_tconf_timer(void);
extern void test_abcsp_txmsg_init_read(ABCSP_TXMSG *msg);
extern unsigned test_abcsp_txmsg_length(ABCSP_TXMSG *msg);
extern char *test_abcsp_txmsg_getbuf(ABCSP_TXMSG *msg, unsigned *buflen);
extern void test_abcsp_txmsg_taken(ABCSP_TXMSG *msg, unsigned ntaken);
extern void test_abcsp_txmsg_done(ABCSP_TXMSG *msg);
extern char *test_abcsp_uart_gettxbuf(unsigned *bufsiz);
extern void test_abcsp_uart_sendbytes(char *buf, unsigned n);
extern void test_abcsp_txmsg_done(ABCSP_TXMSG *msg);

#endif /* ABCSP_TEST */


#endif  /* __ABCSP_TEST_H__ */
