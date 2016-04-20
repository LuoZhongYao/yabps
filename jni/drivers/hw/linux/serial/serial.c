#define TAG "SERIAL"

#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <drv.h>
#include <stddef.h>     /* offset_of */
#include <zl/log.h>
#include <zl/types.h>
#include <assert.h>
#include <zl/util.h>
#include <pthread.h>
#include <stdbool.h>
#include <abcsp/abcsp.h>

struct serial_t{
    int fd;
    enum{
        serial_RUNNING,
        serial_STOPING,
    }status;
    bool running;
} __drv;

#ifdef __LOG_TX_RX__
static int printx(const char *promt,const char *x,size_t n) {
    static char buffer[1024];
    int l = sprintf(buffer,"%s(%02x) : ",promt,n);
    for(size_t i = 0;i < n && i < 1024;i++)
        l += sprintf(buffer + l,"%02x ",x[i]);
    LOGD("%s\n",buffer);
    return 0;
}
#endif


int serial_configure(speed_t rate) {
    struct termios options;
    int val;
    val = tcgetattr(__drv.fd,&options);
    if(val < 0) {
        LOGD("serial_configure %s\n",strerror(errno));
        return -1;
    }
    cfsetispeed(&options,rate);
    cfsetospeed(&options,rate);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag |= (CLOCAL | CREAD);


	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	options.c_oflag &= ~OPOST;
	options.c_oflag &= ~(ONLCR | OCRNL);

	options.c_iflag &= ~(ICRNL | INLCR);
    options.c_iflag &= ~INPCK;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);

	tcflush(__drv.fd, TCIFLUSH);
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

    val = tcsetattr(__drv.fd, TCSANOW, &options);
	if(val < 0) {
        LOGD("serial_configure %s\n",strerror(errno));
		return -1;
	}
    tcflush(__drv.fd, TCIFLUSH);
    return 0;
}

int serial_write(const void *__buffer,size_t __n) {
#ifdef __LOG_TX_RX__
    printx("TX",__buffer,__n);
#endif
    if(__drv.fd > 0)
        return write(__drv.fd,__buffer,__n);
    errno = -ENOENT;
    return -1;
}

int serial_read(void *__buffer,size_t __n) {
    if(__drv.fd > 0)
        return read(__drv.fd,__buffer,__n);
    errno = -ENOENT;
    return -1;
}

int serial_close(void) {
    if(__drv.fd > 0)
        close(__drv.fd);
    return 0;
}

static void* drv_reader(void *__prv) {
    static char buffer[1024];
    int len;

    __drv.status = serial_RUNNING;
    while(__drv.running) {
        len = read(__drv.fd,buffer,sizeof(buffer));
        if(len > 0) {
            int l = 0;
#ifdef  __LOG_TX_RX__
            printx("RX",buffer,len);
#endif
            while(l < len) {
                int il = abcsp_uart_deliverbytes(buffer + l,len - l);
                if(il > 0)
                    l += il;
                else
                    break;
            }
        } else {
            LOGE("%s",strerror(errno));
        }
    }
    __drv.status = serial_STOPING;
    return NULL;
}

int serial_start(void) {
    pthread_t pid;
    __drv.running = true;
    pthread_create(&pid,NULL,drv_reader,NULL);
    return 0;
}

void serial_top(void)
{
    __drv.running = false;
    while(__drv.status == serial_STOPING) 
        usleep(100);
}

int serial_open(const char *path) {
    assert(path != NULL);
    __drv.running = false;
    __drv.fd = open(path,O_RDWR | O_NOCTTY);
    if(__drv.fd < 0) {
        LOGE("%s %s",path,strerror(errno));
        return -1;
    }
    return 0;
}
