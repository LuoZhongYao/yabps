/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2016 Apr 20
 ************************************************/
#ifndef __BTSNOOP_H__
#define __BTSNOOP_H__

#include <stdbool.h>
#include <transport.h>


void btsnoop_open(const char *p_path);
void btsnoop_close(void);
void btsnoop_capture(transport_t channel,const void *p_buf, bool is_rcvd);

#endif

