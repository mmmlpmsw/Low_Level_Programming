#include <stdlib.h>
#include <sys/types.h>

#include "mem.h"

void fill(void* addr, size_t size_bytes) {
    uint8_t* end = addr + size_bytes;
    for (uint8_t* current = addr; current < end; current++)
        *current = (uint8_t) random();
}

void test1() {
    puts("TEST 1:");

    size_t size = 123;
    printf("Allocating %zu bytes...\n", size);
    void* addr = _malloc(size);
    malloc_debug_full_trace();

    puts("Freeing memory...");
    _free(addr);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test2() {
    puts("TEST 2:");

    size_t size = 76;
    void* addrs[10];
    printf("Allocating %zu bytes 10 times...\n", size);
    for (int i = 0; i < 10; i++) {
        addrs[i] = _malloc(size);
    }
    malloc_debug_full_trace();
    puts("Freeing all memory...");
    for (int i = 0; i < 10; i++) {
        _free(addrs[i]);
    }
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test3() {
    puts("TEST 3:");
    size_t size = 109345;
    printf("Allocating %zu bytes...\n", size);
    void* addr = _malloc(size);
    malloc_debug_full_trace();
    puts("Freeing memory...");
    _free(addr);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test4() {
    puts("TEST 4:");
    size_t size = 6000000000;
    printf("Allocating %zu bytes...\n", size);
    void* addr = _malloc(size);

    if (addr) {
        fill(addr, size);
        printf("Allocated at %p\n", addr);
    } else
        printf("Memory did NOT allocated\n");

    malloc_debug_full_trace();
    puts("Freeing memory...");
    _free(addr);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test5() {
    puts("TEST 5:");
    size_t size = 16000000000;
    printf("Allocating %zu bytes...\n", size);
    void* addr = _malloc(size);

    if (addr) {
        fill(addr, size);
        printf("Allocated at %p\n", addr);
    } else
        printf("Memory did NOT allocated\n");

    malloc_debug_full_trace();
    puts("Freeing memory...");
    _free(addr);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test6() {
    puts("TEST 6:");
    size_t size = 0;
    printf("Allocating %zu bytes...\n", size);
    void* addr = _malloc(size);

    if (addr)
        printf("Allocated at %p\n", addr);
    else
        printf("Memory did NOT allocated\n");

    malloc_debug_full_trace();
    puts("Freeing memory...");
    _free(addr);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test7() {
    puts("TEST 7:");
    size_t size1 = 235, size2 = 2235, size3 = 35283;
    printf("Allocating %zu, %zu and %zu bytes...\n", size1, size2, size3);
    void* addr1 = _malloc(size1);
    void* addr2 = _malloc(size2);
    void* addr3 = _malloc(size3);
    printf("Freeing middle space...\n");
    _free(addr2);
    malloc_debug_full_trace();

    puts("Freeing memory...");
    _free(addr1);
    _free(addr3);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test8() {
    puts("TEST 8:");
    size_t size1 = 1837192623, size2 = 3536732523, size3 = 2462372345;
    printf("Allocating %zu, %zu and %zu bytes...\n", size1, size2, size3);
    void* addr1 = _malloc(size1);
    void* addr2 = _malloc(size2);
    void* addr3 = _malloc(size3);
    printf("Freeing middle space...\n");
    _free(addr2);
    malloc_debug_full_trace();

    puts("Freeing memory...");
    _free(addr1);
    _free(addr3);
    malloc_debug_full_trace();
    malloc_debug_full_reset();
}

void test9() {
    puts("TEST 9:");
    printf("Allocating -1 bytes...\n");
    void* addr = _malloc(-1);

    if (addr) {
        printf("Allocated at %p\n", addr);
        fill(addr, 1000000);

        malloc_debug_full_trace();

        puts("Freeing memory...");
        _free(addr);
    } else
        printf("Memory did NOT allocated\n");

    malloc_debug_full_trace();
    malloc_debug_full_reset();
}