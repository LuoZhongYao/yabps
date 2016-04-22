/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2016 Apr 22
 ************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>

struct timer;

extern void timer_init(void);
extern void timer_tick(void);
extern bool mktimer(struct timer **timer,long delay,void (*fn)(void *),void *data);
extern void deltimer(struct timer *timer);


#endif

