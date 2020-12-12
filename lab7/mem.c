#include <stdbool.h>
#include <stdio.h>
#include "mem.h"
#include <sys/mman.h>
#include <stdint.h>

void* HEAP_START = NULL;

struct __attribute__((packed)) mem_header {
    struct mem_header* next;
    size_t capacity;
    bool is_free;
};

void divide(mem_header* current, size_t query) {
    mem_header* next = (mem_header*)((uint8_t*)current + query + sizeof(mem_header));
    size_t next_size = current->capacity - sizeof(mem_header) - query;
    next->capacity = next_size;
    next->next = NULL;
    next->is_free = true;
    current->is_free = false;
    current->next = next;
    current->capacity = query;
}

void* _malloc(size_t query) {
    if (query + sizeof(struct mem_header) < query)
        return NULL;

    mem_header * header = HEAP_START;
    while (header && (header->capacity < query || !header->is_free))
        header = header->next;

    if (!header) {
        header = mmap(NULL, query + sizeof(mem_header), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (header == MAP_FAILED)
            return NULL;

        if (!HEAP_START)
            HEAP_START = header;

        mem_header* prev = HEAP_START;
        while (prev->next != NULL) {
            prev = prev->next;
        }
        prev->next = header;
        header->capacity = query;
        header->is_free = false;
        header->next = NULL;
    } else {
        divide(header, query);
    }
    return header + 1;
}

void _free(void* ptr) {
    mem_header* pointer = ptr;
    (pointer - 1)->is_free = true;
}

void* get_node_end(struct mem_header * node) {
    if (node == NULL)
        return NULL;

    int8_t* body_start = (int8_t*)(node+1);
    return body_start + node->capacity;
}

void malloc_debug_full_trace() {
    puts("                    HEAP TRACE");

    struct mem_header * current = HEAP_START;

    if (current == NULL) {
        puts("──────────────────────────────────────────────────");
        puts("\033[38;5;8m                     (empty)\033[0m");
        puts("──────────────────────────────────────────────────");
        return;
    } else {
        puts("────┬──────────────────┬───────────────────┬──────");
    }

    int i = 0;
    while (current) {
        printf(
                "%3d │ %16p │ %11zu bytes │ %4s\n",
                i + 1,
                current+1,
                current->capacity,
                current->is_free ? "\033[38;5;40mFREE\033[0m" : "\033[38;5;196mBUSY\033[0m"
        );

        if (current->next)
            puts("────┼─ ── ── ── ── ── ─┼── ── ── ─ ── ── ──┼──────");
        else
            puts("────┴──────────────────┴───────────────────┴──────");

        if (current->next && current->next != get_node_end(current)) {
            puts("\033[38;5;8m                 ~~ memory gap ~~\033[0m");
            puts("────┼─ ── ── ── ── ── ─┼── ── ── ─ ── ── ──┼──────");
        }

        i++;
        current = current->next;
    }
}

void malloc_debug_full_reset() {
    mem_header* current = HEAP_START;
    while (current != NULL) {
        mem_header* to_free = current;
        current = current->next;
        munmap(to_free, sizeof(mem_header) + to_free->capacity);
    }
    HEAP_START = NULL;
}