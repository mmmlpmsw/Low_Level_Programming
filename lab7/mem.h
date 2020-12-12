#ifndef AAAAAAAAAA_MEM_H
#define AAAAAAAAAA_MEM_H

#include <stddef.h>
#include <stdbool.h>


#define MIN_BLOCK_SIZE 32
#define DEBUG_FIRST_BYTES 4

struct mem_header;
typedef struct mem_header mem_header;

void* _malloc(size_t query);
void  _free(void* mem_header);

void malloc_debug_full_trace();
void malloc_debug_full_reset();

#endif //AAAAAAAAAA_MEM_H
