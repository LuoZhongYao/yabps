#include <list.h>
#include <unistd.h>
#include <stdlib.h>
#include <zl/util.h>
#include <timer.h>

struct timer {
    void (*fn)(void *);
    void *data;
    struct list_head list;
    long  delay;
};

#define WHEEL_TICK    (30)
#define WHEEL0_TICK   WHEEL_TICK
#define WHEEL1_TICK   (WHEEL0_TICK * WHEEL0_TICK)
#define WHEEL2_TICK   (WHEEL1_TICK * WHEEL0_TICK)
#define WHEEL3_TICK   (WHEEL2_TICK * WHEEL0_TICK)
#define WHEEL4_TICK   (WHEEL3_TICK * WHEEL0_TICK)

struct timing_wheel
{
    int     port;
    struct list_head fns[WHEEL0_TICK];
};

static struct timing_wheel wheels[4];

static void wheel_insert(struct timer *timer,struct timing_wheel *wheel,int offset)
{
    offset = (wheel->port + offset) % WHEEL_TICK;
    list_add(&timer->list,wheel->fns + offset);
}

static bool timer_insert(struct timer *timer)
{
    int offset = 0;
    if(timer->delay < WHEEL0_TICK) {
        offset = timer->delay;
        timer->delay = 0;
        wheel_insert(timer,wheels,offset);
    } else if(timer->delay < WHEEL1_TICK) {
        offset = timer->delay / WHEEL0_TICK;
        timer->delay -= (offset * WHEEL0_TICK);
        wheel_insert(timer,wheels + 1,offset);
    } else if(timer->delay < WHEEL2_TICK) {
        offset = timer->delay / WHEEL1_TICK;
        timer->delay -= (offset * WHEEL1_TICK);
        wheel_insert(timer,wheels + 2,offset);
    } else if(timer->delay < WHEEL3_TICK) {
        offset = timer->delay / WHEEL2_TICK;
        timer->delay -= (offset * WHEEL2_TICK);
        wheel_insert(timer,wheels + 3,offset);
    } else {
        return false;
    }
    return true;
}

static bool timer_add(struct timer *timer)
{
    if(timer->delay < WHEEL0_TICK) {
    } else if(timer->delay < WHEEL1_TICK) {
        timer->delay += wheels[0].port;
    } else if(timer->delay < WHEEL2_TICK) {
        timer->delay += (wheels[0].port + (wheels[1].port * WHEEL0_TICK));
    } else if(timer->delay < WHEEL3_TICK) {
        timer->delay += (wheels[0].port + (wheels[1].port * WHEEL0_TICK) + (wheels[2].port * WHEEL1_TICK));
    } else {
        return false;
    }
    return timer_insert(timer);
}


void timer_init(void)
{
    for(int i = 0;i < 4;i++) {
        wheels[i].port = 0;
        for(int j = 0;j < WHEEL0_TICK;j++) {
            INIT_LIST_HEAD(wheels[i].fns + j);
        }
    }
}

void timer_tick(void)
{
    struct timing_wheel *wheel = NULL;
    for(int i = 0;i < 4;i++) {
        wheel = wheels + i;
        wheel->port = (wheel->port + 1) % WHEEL_TICK;
        list_for_each_safe(pos,next,wheel->fns + wheel->port) {
            list_del(pos);
            struct timer *timer = container_of(pos,struct timer,list);
            if(timer->delay) {
                timer_insert(timer);
            } else {
                timer->fn(timer->data);
                free(timer);
            }
        }
        if(wheel->port)
            break;
    }
}

bool mktimer(struct timer **timer,long delay,void (*fn)(void *),void *data)
{
    struct timer *t = malloc(sizeof(*t));
    INIT_LIST_HEAD(&t->list);
    t->fn = fn;
    t->delay = delay;
    t->data = data;
    if(timer_add(t)) {
        if(timer) {
            *timer = t;
        }
        return true;
    }
    free(t);
    return false;
}

void deltimer(struct timer *timer)
{
    if(timer) {
        list_del(&(timer->list));
        free(timer);
    }
}

#ifdef __TIMER_TEST__
#include <stdio.h>
int tick = 0;
static void test(int i)
{
    printf("------------------------ tick %d,%d\n",tick,i);
}

int main(void)
{
    timer_init();
#define MKTIME(t)   mktimer(NULL,t,(void*)test,(void*)t)
    MKTIME(10);
    MKTIME(29);
    MKTIME(30);
    MKTIME(31);

    MKTIME(899);
    MKTIME(900);
    MKTIME(901);

    MKTIME(26999);
    MKTIME(27000);
    MKTIME(27001);

    MKTIME(809999);
    MKTIME(810000);
    MKTIME(810001);

    while(tick < 810002) {
        usleep(100);
        tick++;
        timer_tick();
        if(tick == 27001) {
            MKTIME(900);
        }
    }
}
#endif

