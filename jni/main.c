#include "zl/log.h"
#include "arch/rda/5876/rda5876.h"
#include "drivers/abcsp/include/abcsp.h"
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "drv.h"
#include "hci.h"
#include "zl/types.h"
extern drv_t *linux_serial(const char *path,drv_t *top);
drv_t *drv,*abcsp,*hci;

static void usage(const char *cmd) {
    LOGE("Usage : %s devices\n",cmd);
}

int abcsp_uart_sendbytes(void *__buffer,size_t __n) {
    return drv_send(drv,0,__buffer,__n);
}

void *abcsp_uart_gettxbuf(size_t *__n) {
    static u8 buffer[1024];
    *__n = 1024;
    return buffer;
}

extern void abscp_uart_receiv(MessageStructure *src,unsigned channel)
{
    drv_receiv(hci,src->buf,0);
}

int main(int argc,char **argv) {
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }
    rdabt_poweron_init(argv[1]);
    hci = NEW(hci,NULL);
    abcsp = new_abcsp(hci);
    drv = linux_serial(argv[1],abcsp);
    drv_configure(drv,B500000);
    while(1) 
        sleep(1);
    /*************************************************/

    delete_abcsp(abcsp);
    delete_hci(hci);
    return 0;
}
