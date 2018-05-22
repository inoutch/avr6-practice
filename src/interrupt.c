#include "interrupt.h"
#include "lib.h"

int softvec_init(void) {
    int type;
    for (type = 0; type < SOFTVEC_TYPE_NUM; type++)
        softvec_setintr(type, NULL);
    return 0;
}

int softvec_setintr(softvec_type_t type, softvec_handler_t handler) {
    SOFTVECS[type] = handler;
    return 0;
}

void nnn() {
    //putc('c');
}

void interrupt(softvec_type_t type, uint16_t sp) {
    //softvec_handler_t handler = SOFTVECS[type];
    //if (handler) {
    //hello(type, sp);
    //handler(type, sp);
    //}
}