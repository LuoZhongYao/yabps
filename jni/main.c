#include "arch/rda/5876/rda5876.h"
#include "drivers/abcsp/include/abcsp.h"
#include "drivers/abcsp/include/abcsp_events.h"
#include "drivers/abcsp/include/abcsp_panics.h"
#include <termio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "drv.h"
#include "zl/types.h"
#include "zl/log.h"
extern drv_t *linux_serial(const char *path);
drv_t *drv;

static void usage(const char *cmd) {
    LOGE("Usage : %s devices\n",cmd);
}

static int printx(const char *promt,u8 *x,size_t n) {
    static char buffer[1024];
    int l = sprintf(buffer,"%s(%02x) : ",promt,n);
    for(size_t i = 0;i < n && i < 1024;i++)
        l += sprintf(buffer + l,"%02x ",x[i]);
    LOGD("%s\n",buffer);
    return 0;
}

static void* drv_reader(void *__prv) {
    static u8 buffer[1024];
    int len;

    drv_t *drv = __prv;
    LOGD("drv_reader");
    while(1) {
        len = drv_read(drv,buffer,sizeof(buffer));
        if(len > 0) {
            int l = 0;
            printx("RX",buffer,len);
            while(l < len) {
                l += abcsp_uart_deliverbytes((char*)buffer + l,len);
            }
        } else {
            LOGE("%s",strerror(errno));
        }
    }
    return NULL;
}

int abcsp_uart_sendbytes(void *__buffer,size_t __n) {
    printx("TX",__buffer,__n);
    return drv_write(drv,__buffer,__n);
}

void *abcsp_uart_gettxbuf(size_t *__n) {
    static u8 buffer[1024];
    *__n = 1024;
    return buffer;
}

int main(int argc,char **argv) {
    pthread_t pid;
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }
    rdabt_poweron_init(argv[1]);
    drv = linux_serial(argv[1]);
    drv_configure(drv,B500000);
    pthread_create(&pid,NULL,drv_reader,drv);
    abcsp_init();
    while(1) 
        sleep(1);
    return 0;
}
