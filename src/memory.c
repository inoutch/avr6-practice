#include "lib.h"
#include "os.h"
#include "memory.h"

typedef struct _mem_block {
    struct _mem_block *next;
    int size;
} mem_block;

typedef struct _mem_pool {
    int size;
    int num;
    mem_block *free;
} mem_pool;

static mem_pool pool[] = {
        // size, num, free
        {16, 8, NULL},
        {32, 8, NULL},
        {64, 4, NULL},
};

#define MEMORY_AREA_NUM (sizeof(pool) / sizeof(*pool))

static int mem_init_pool(mem_pool *p) {
    int i;
    mem_block *m_block_ptr;
    mem_block **m_block_ptr_ptr;
    extern char _freearea;
    static char *area = &_freearea;

    m_block_ptr = (mem_block *) area;// メモリプールの開始16, 32, 64

    m_block_ptr_ptr = &p->free;// free
    for (i = 0; i < p->num; i++) {
        *m_block_ptr_ptr = m_block_ptr;// free..next..next
        memset(m_block_ptr, 0, sizeof(*m_block_ptr));
        m_block_ptr->size = p->size;//m_block_size = 16 or 32 or 64
        m_block_ptr_ptr = &(m_block_ptr->next);
        m_block_ptr = (mem_block *) ((char *) m_block_ptr + p->size);// next
        area += p->size;
    }
    return 0;
}

int kernel_mem_init(void) {
    int i;
    for (i = 0; i < MEMORY_AREA_NUM; i++) {
        mem_init_pool(&pool[i]);
    }
    return 0;
}

void *kernel_malloc(int size) {
    int i;
    mem_block *mp;
    mem_pool *p;

    for (i = 0; i < MEMORY_AREA_NUM; i++) {
        p = &pool[i];
        if (size <= p->size - (int) sizeof(mem_block)) {
            if (p->free == NULL) {
                kernel_shutdown();
                return NULL;
            }
            mp = p->free;
            p->free = p->free->next;
            mp->next = NULL;
            return mp + 1;
        }
    }

    kernel_shutdown();
    return NULL;
}

void kernel_free(void *mem) {
    int i;
    mem_block *mb;
    mem_pool *mp;

    mb = ((mem_block *) mem - 1);

    for (i = 0; i < MEMORY_AREA_NUM; i++) {
        mp = &pool[i];
        if (mb->size == mp->size) {
            mb->next = mp->free;
            mp->free = mb;
            return;
        }
    }

    kernel_shutdown();
}