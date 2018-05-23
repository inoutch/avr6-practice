#include "os.h"
#include "lib.h"
#include "memory.h"
#include "interrupt.h"

#define THREAD_NUM 6
#define PRIORITY_NUM 4

typedef struct {
    struct context_t *next;
    int priority;
    char *stack;
    uint32_t flags;

    thread_func func;
    struct {
        int argc;
        char *argv;
    } init;

    struct {
        syscall_type type;
        syscall_params_t *params;
    } syscall_info;

    uint16_t sp;
} context_t;

static struct {
    context_t *head;
    context_t *tail;
} ready_context_queue[PRIORITY_NUM];

static context_t main = {};
static context_t *current = &main;
static context_t contexts[THREAD_NUM];

// user
// memory
void *malloc(int size) {
    syscall_params_t params;
    params.un.malloc.size = size;
    syscall(SYSCALL_TYPE_MALLOC, &params);
    return params.un.malloc.ret;
}

void free(void *ptr) {
    syscall_params_t params;
    params.un.free.ptr = ptr;
    syscall(SYSCALL_TYPE_FREE, &params);
}

// thread
void exit(int ret) {
    syscall_params_t params;
    params.un.exit.ret = ret;
    syscall(SYSCALL_TYPE_EXIT, &params);
}

thread_id_t thread_run(thread_func func, int priority, int stacksize, int argc, char *argv[]) {
    syscall_params_t params;
    params.un.run.func = func;
    params.un.run.priority = priority;
    params.un.run.stacksize = stacksize;
    params.un.run.argc = argc;
    params.un.run.argv = argv;
    syscall(SYSCALL_TYPE_RUN, &params);
    return params.un.run.id;
}

void wakeup(thread_id_t id) {
    syscall_params_t params;
    params.un.wakeup.id = id;
    syscall(SYSCALL_TYPE_WAKEUP, &params);
}

void sleep(uint16_t milliseconds) {
    syscall_params_t params;
    params.un.sleep.milliseconds = milliseconds;
    syscall(SYSCALL_TYPE_SLEEP, &params);
}

// thread
void kernel_queue_context() {
    if (current == NULL) {
        return;
    }
    if (ready_context_queue[current->priority].tail) {
        ready_context_queue[current->priority].tail->next = current;
    } else {
        ready_context_queue[current->priority].head = current;
    }
    ready_context_queue[current->priority].tail = current;
    // three
    // priority
    // 0 -> head: thread1 -> thread2 -> tail: thread3 <- append new thread
    // ~ -> ...
    // 3 -> ...
}

void kernel_dequeue_context() {
    if (current == NULL) {
        return;
    }
    ready_context_queue[current->priority].head = current->next;
    if (ready_context_queue[current->priority].tail == NULL) {
        ready_context_queue[current->priority].head = NULL;
    }
    current->next = NULL;
}

void kernel_thread_init(context_t *context) {
    context->func(context->init.argc, context->init.argv);
    kernel_thread_end();
}

void kernel_thread_end() {
    exit(0);
}

thread_id_t kernel_thread_run(int priority, thread_func func, int argc, char *argv, int stacksize) {
    int i;
    context_t *context_ptr;
    uint8_t *sp;
    extern char _userstack;
    static char *thread_stack = &_userstack;

    // find unused thread
    for (i = 0; i < THREAD_NUM; i++) {
        context_ptr = &contexts[i];
        if (!context_ptr->func) {
            break;
        }
    }
    // contexts are full
    if (i == THREAD_NUM) {
        return -1;
    }

    // create thread context
    memset(context_ptr, 0, sizeof(*context_ptr));

    context_ptr->next = NULL;
    context_ptr->priority = priority;
    context_ptr->flags = 0;

    context_ptr->func = func;
    context_ptr->init.argc = argc;
    context_ptr->init.argv = argv;

    // allocation stack
    memset(thread_stack, 0, stacksize);
    thread_stack += stacksize;

    // set start ptr of stack
    context_ptr->stack = thread_stack;

    sp = (uint8_t *) context_ptr->stack;
    // *(sp--) = is stack value
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_end >> 0) & 0xff);
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_end >> 8) & 0xff);

    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_init >> 0) & 0xff);
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_init >> 8) & 0xff);
    // thread stack
    // --
    // thread_init
    // thread_end

    // stack r31~r26
    for (i = 31; i >= 26; i--) {
        *(sp--) = 0;
    }

    // stack context argument for thread_init(-- here --)
    *(sp--) = (uint8_t) (((uint16_t) context_ptr >> 8) & 0xff);  /* r25 */
    *(sp--) = (uint8_t) (((uint16_t) context_ptr >> 0) & 0xff);  /* r24 */

    // stack r23~r0
    for (i = 23; i >= 0; i--) {
        *(sp--) = 0;
    }

    // ?
    *(sp--) = 0;
    *(sp--) = 0;

    // set sp to context
    context_ptr->sp = (uint16_t) sp;

    // evacuation current context
    kernel_queue_context();

    // set new context
    current = context_ptr;

    return (thread_id_t) current;
}

void kernel_thread_exit() {
    if (current != NULL) {
        memset(current, 0, sizeof(current));
    }
}

void kernel_thread_wait() {
}

void kernel_thread_wakeup() {
}

void kernel_thread_sleep() {
}

void kernel_shutdown() {
    puts("\nsystem error!\n");
    while (1);
}

// bind interrupts
void timer_intr(softvec_type_t type, uint16_t sp) {
}

void syscall_intr(softvec_type_t type, uint16_t sp) {
    switch (current->syscall_info.type) {
        case SYSCALL_TYPE_EXIT:
            kernel_thread_exit();
            break;
        case SYSCALL_TYPE_RUN:
            kernel_thread_run(
                    current->syscall_info.params->un.run.func,
                    current->syscall_info.params->un.run.priority,
                    current->syscall_info.params->un.run.stacksize,
                    current->syscall_info.params->un.run.argc,
                    current->syscall_info.params->un.run.argv);
            break;
        case SYSCALL_TYPE_SLEEP:
            kernel_thread_sleep();
            break;
        case SYSCALL_TYPE_WAIT:
            kernel_thread_wait();
            break;
        case SYSCALL_TYPE_WAKEUP:
            kernel_thread_wakeup();
            break;
        case SYSCALL_TYPE_MALLOC:
            current->syscall_info.params->un.malloc.ret = kernel_malloc(current->syscall_info.params->un.malloc.size);
            break;
        case SYSCALL_TYPE_FREE:
            kernel_free(current->syscall_info.params->un.free.ptr);
            break;
        default:
            break;
    }
}

void os_init() {
    INTR_DISABLE;

    // init serial
    init();

    // init memory
    kernel_mem_init();

    // init interrupt
    softvec_init();
    softvec_setintr(SOFTVEC_TYPE_SYSCALL, syscall_intr);
    softvec_setintr(SOFTVEC_TYPE_TIMEINT, timer_intr);

    // init timer interrupt
    timer_interrupt_init();

    INTR_ENABLE;
}

void syscall(syscall_type type, syscall_params_t *params) {
    void intr_syscall();
    INTR_DISABLE;
    current->syscall_info.type = type;
    current->syscall_info.params = params;
    intr_syscall();
}