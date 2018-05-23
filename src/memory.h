#ifndef _MEMORY_H_INCLUDED_
#define _MEMORY_H_INCLUDED_

int kernel_mem_init(void);

void *kernel_malloc(int size);

void kernel_free(void *mem);

#endif
