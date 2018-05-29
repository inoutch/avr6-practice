#ifndef __OS_H__
#define __OS_H__

#include "type.h"

typedef enum {
    SYSCALL_TYPE_EXIT = 0,
    SYSCALL_TYPE_RUN,
    SYSCALL_TYPE_WAIT,
    SYSCALL_TYPE_SLEEP,
    SYSCALL_TYPE_WAKEUP,
    SYSCALL_TYPE_GETID,
    SYSCALL_TYPE_MALLOC,
    SYSCALL_TYPE_FREE,
} syscall_type;

// thread
typedef int (*thread_func)(int argc, char *argv[]);

typedef uint16_t thread_id_t;

typedef struct {
    union {
        struct {
            int size;
            void *ret;
        } malloc;

        struct {
            void *ptr;
        } free;

        struct {
            int ret;
        } exit;

        struct {
            thread_func func;
            int priority;
            int stacksize;
            int argc;
            char **argv;
            thread_id_t id;
        } run;

        struct {
            thread_id_t id;
        } wakeup;

        struct {
            uint16_t milliseconds;
        } sleep;
    } un;
} syscall_params_t;

// user
void os_init();

void syscall(syscall_type type, syscall_params_t *params);

void *malloc(int size);

void free(void *ptr);

void exit(int ret);


// thread
thread_id_t thread_run(thread_func func, int stacksize, int argc, char *argv[]);

void thread_sleep();

// kernel
void kernel_shutdown();

#endif