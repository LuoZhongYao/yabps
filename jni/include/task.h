#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <zl/types.h>
#include <list.h>
__BEGIN_DECLS

typedef int     message_id_t;
typedef void*   message_t;
typedef struct  task_t task_t;
typedef struct  message_list_t message_list_t;
typedef int (*message_handler_t)(task_t *task,message_id_t id,message_t msg);

struct task_t {
    char *name;
    message_handler_t handler;
};

#define NEW(__obj,...)       new_ ## __obj(__VA_ARGS__)
#define DELETE(__obj,...)    delete_ ## __obj(__VA_ARGS__)
#define METHON(__obj,__methon,__defualt,...)    ({\
        __typeof__((__obj)->__methon((__obj),##__VA_ARGS__)) _v =\
        (__defualt);\
        if((__obj) && (__obj)->__methon) { \
            _v = (__obj)->__methon((__obj),##__VA_ARGS__);\
        }_v;})


__END_DECLS
#endif
