#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

void init(void);

int putc(char c);

int puts(char *str);

char getc();

int puthex(char c);

int send(char *c, unsigned int size);

#endif
