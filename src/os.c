#include "os.h"
#include "lib.h"
#include "memory.h"
#include "interrupt.h"

#define THREAD_NUM 6

#define WINDOW_SIZE 128

typedef struct {
    struct context_t *next;
    char *stack;

    struct {
        thread_func func;
        int argc;
        char *argv;
    } init;

    struct {
        syscall_type type;
        syscall_params_t *params;
    } syscall_info;

    uint16_t sp;
} context_t;

static context_t *current;
static context_t contexts[THREAD_NUM];
static struct {
    context_t *head;
    context_t *tail;
} ready_context_queue;


void kernel_queue_context();

void dispatch(context_t *context);

context_t *kernel_schedule();

void kernel_thread_resume(context_t *context);


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
void thread_init() {
    current = contexts;
    ready_context_queue.head = ready_context_queue.tail = current;
}

void exit(int ret) {
    syscall_params_t params;
    params.un.exit.ret = ret;
    syscall(SYSCALL_TYPE_EXIT, &params);
}

thread_id_t thread_run(thread_func func, int stacksize, int argc, char *argv[]) {
    syscall_params_t params;
    params.un.run.func = func;
    params.un.run.stacksize = stacksize;
    params.un.run.argc = argc;
    params.un.run.argv = argv;
    syscall(SYSCALL_TYPE_RUN, &params);
    return params.un.run.id;
}

void sleep(uint16_t milliseconds) {
    syscall_params_t params;
    params.un.sleep.milliseconds = milliseconds;
    syscall(SYSCALL_TYPE_SLEEP, &params);
}


// kernel
// thread
void kernel_queue_context(context_t *context) {
    if (context == NULL) {
        return;
    }
    if (ready_context_queue.tail) {
        ready_context_queue.tail->next = context;
    } else {
        ready_context_queue.head = context;
    }
    ready_context_queue.tail = context;
}

void kernel_remove_context(context_t *context) {
    context_t *c = ready_context_queue.head;
    if (c == context) {
        if (c->next == NULL) {
            kernel_shutdown();// last thread ended
        }
        ready_context_queue.head = c->next;
        memset(context, 0, sizeof(context));
        return;
    }
    while (c) {
        if (c->next == current) {
            c->next = current->next;
            if (c->next == NULL) {
                ready_context_queue.tail = c;
            }
            break;
        }
        c = c->next;
    }
    memset(context, 0, sizeof(context));
}

// remove current context
void kernel_thread_end() {
    INTR_DISABLE;
    context_t *next = kernel_schedule();
    kernel_remove_context(current);
    kernel_thread_resume(next);
}

void kernel_thread_init(context_t *context) {
    context->init.func(context->init.argc, context->init.argv);
    //kernel_thread_end(); maybe it is not necessary because the returning address of kernel_thread_end stacks.
}

thread_id_t kernel_thread_run(thread_func func, int stacksize, int argc, char *argv) {
    int i;
    context_t *context_ptr;
    uint8_t *sp;
    extern char _userstack;
    static char *thread_stack = &_userstack;

    // find unused thread
    for (i = 1; i < THREAD_NUM; i++) {
        context_ptr = &contexts[i];
        if (!context_ptr->init.func) {
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
    context_ptr->init.func = func;
    context_ptr->init.argc = argc;
    context_ptr->init.argv = argv;

    // allocation stack manually
    memset(thread_stack - stacksize, 0, stacksize);

    // set start ptr of stack
    context_ptr->stack = thread_stack;
    thread_stack -= stacksize;

    sp = (uint8_t *) context_ptr->stack;
    // *(sp--) = is stack value
    // push returning addr
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_end >> 0) & 0xff);
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_end >> 8) & 0xff);
    *(sp--) = (uint8_t) 0x00;

    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_init >> 0) & 0xff);
    *(sp--) = (uint8_t) (((uint16_t) kernel_thread_init >> 8) & 0xff);
    *(sp--) = (uint8_t) 0x00;
    // thread stack
    // -~~~-
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

    //
    *(sp--) = 0; // RAMPZ
    *(sp--) = 0; // EIND
    *(sp--) = 0; // SREG

    // set sp to context
    context_ptr->sp = (uint16_t) sp;

    // evacuation current context
    kernel_queue_context(context_ptr);

    return (thread_id_t) current;
}

void kernel_thread_exit() {
    if (current != NULL) {
        memset(current, 0, sizeof(current));
    }
}

void kernel_thread_resume(context_t *context) {
    current = context;
    dispatch(&current->sp);
}

void kernel_shutdown() {
    puts("\nsystem error!\n");
    while (1);
}

context_t *kernel_schedule() {
    return current->next ? current->next : ready_context_queue.head;
}


// bind interrupts
void timer_intr(softvec_type_t type, uint16_t sp) {
    current->sp = sp;
    context_t *next = kernel_schedule();
    if (next != NULL) {
        kernel_thread_resume(next);
    }
}

void syscall_intr(softvec_type_t type, uint16_t sp) {
    switch (current->syscall_info.type) {
        case SYSCALL_TYPE_EXIT:
            kernel_thread_exit();
            break;
        case SYSCALL_TYPE_RUN:
            kernel_thread_run(
                    current->syscall_info.params->un.run.func,
                    current->syscall_info.params->un.run.stacksize,
                    current->syscall_info.params->un.run.argc,
                    current->syscall_info.params->un.run.argv);
            break;
        case SYSCALL_TYPE_SLEEP:
            //kernel_thread_sleep(0);
            break;
        case SYSCALL_TYPE_WAIT:
            //kernel_thread_wait();
            break;
        case SYSCALL_TYPE_WAKEUP:
            //kernel_thread_wakeup();
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

    // init thread
    thread_init();

    INTR_ENABLE;
}

void syscall(syscall_type type, syscall_params_t *params) {
    void intr_syscall();
    INTR_DISABLE;
    current->syscall_info.type = type;
    current->syscall_info.params = params;
    intr_syscall();
}