#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

#include "type.h"

void init(void);

// serial
int putc(char c);

int puts(char *str);

int puthex(char c);

char getc();

int send(char *c, unsigned int size);

// memory
void memset(void *data, int value, uint32_t size);

void *alloc(uint32_t size);

void free(void *ptr);

#endif
