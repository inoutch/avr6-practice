#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

#include "type.h"

void init(void);

// serial
int putc(char c);

int puts(char *str);

int puthex(char c);

int putp(void *ptr);

int puti(int i);

char getc();

int send(char *c, unsigned int size);

void *memset (void *b, int c, long len);

char *strcpy(char *dst, const char *src);

#endif
