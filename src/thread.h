#ifndef _THREAD_H_INCLUDED_
#define _THREAD_H_INCLUDED_

#include "type.h"

typedef uint16_t thread_id_t;

typedef int (*func_t)(int argc, char *argv[]);

typedef void (*handler_t)(void);

typedef struct {
    thread_id_t id;
} thread_ptr;

void thread_create(thread_ptr *ptr, func_t func, int argc, char *argv[]);

#endif