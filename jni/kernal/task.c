#include <task.h>
#include <assert.h>

int message_send(task_t *task,message_id_t id,message_t msg)
{
    assert(task);
    assert(task->handler);
    return task->handler(task,id,msg);
}

int message_send_later(task_t *task,message_id_t id,message_t msg,time_t msdelay)
{
    return 0;
}

int message_cannel_all(task_t *task,message_id_t id)
{
    return 0;
}

int handle_later(time_t msdelay,void(*handler)(void *args),void *args)
{
    return 0;
}
