#ifndef _INTERRUPT_H_INCLUDED_
#define _INTERRUPT_H_INCLUDED_

#include "type.h"

#define SOFTVEC_TYPE_NUM     4
#define SOFTVEC_TYPE_SOFTERR 0
#define SOFTVEC_TYPE_SYSCALL 1
#define SOFTVEC_TYPE_SERINTR 2
#define SOFTVEC_TYPE_TIMEINT 3

#define INTR_ENABLE  asm volatile ("sei")
#define INTR_DISABLE asm volatile ("cli")

extern char _softvec; // defined by ld.scr
#define SOFTVEC_ADDR (&_softvec)
#define SOFTVECS ((softvec_handler_t *)SOFTVEC_ADDR)

typedef short softvec_type_t;

typedef void (*softvec_handler_t)(softvec_type_t type, uint16_t sp);

int softvec_init(void);

int softvec_setintr(softvec_type_t type, softvec_handler_t handler);

void interrupt(softvec_type_t type, uint16_t sp);

#endif