#define TAG "ZL.MAIN"

#include <zl/log.h>
#include "arch/rda/5876/rda5876.h"
#include "drivers/hw/abcsp/include/abcsp.h"
#include <termio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <drv.h>
#include <hci.h>
#include <l2cap.h>
#include <sdp.h>
#include <zl/types.h>
#include <timer.h>

#define work_loop() while(1) sleep(1)


extern int serial_open(const char *path);
extern int serial_configure();
extern int serial_write();
extern void serial_start();
extern void serial_stop();


static void usage(const char *cmd) {
    LOGE("Usage : %s devices\n",cmd);
}

static void mktick(void)
{
    struct itimerval va = {
        .it_value.tv_sec = 0,
        .it_value.tv_usec = 1000,
    };
    va.it_interval = va.it_value;
    setitimer(ITIMER_REAL,&va,NULL);
}

static void tick_handler(int sig __unused)
{
    timer_tick();
}

static void mksig(void)
{
    struct sigaction act = {
        .sa_handler = tick_handler,
        .sa_flags = SA_RESTART,
    };
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    mktick();
}

int main(int argc,char **argv) {
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }

    timer_init();
    mksig();

    rdabt_poweron_init(argv[1]);
    /*********** new ********************/
    serial_open(argv[1]);
    serial_configure(B500000);
    /************ ***********************/
    serial_start();
    abcsp_init();
    sdp_init();

    work_loop();

    serial_stop();
    return 0;
}
