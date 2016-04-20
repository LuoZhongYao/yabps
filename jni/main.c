#define TAG "MAIN"

#include <zl/log.h>
#include "arch/rda/5876/rda5876.h"
#include "drivers/hw/abcsp/include/abcsp.h"
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <drv.h>
#include <hci.h>
#include <l2cap.h>
#include <zl/types.h>
extern int serial_open(const char *path);
extern int serial_configure();
extern int serial_write();
extern void serial_start();
extern void serial_stop();


static void usage(const char *cmd) {
    LOGE("Usage : %s devices\n",cmd);
}

int main(int argc,char **argv) {
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }
    rdabt_poweron_init(argv[1]);
    /*********** new ********************/
    serial_open(argv[1]);
    serial_configure(B500000);
    /************ ***********************/
    serial_start();
    abcsp_init();
    register_l2cap_listen(SDP_PSM,NULL,NULL);
    while(1) 
        sleep(1);
    serial_stop();
    return 0;
}
