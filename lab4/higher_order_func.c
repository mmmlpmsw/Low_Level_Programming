#include "./linked_list.h"
#include "./higher_order_func.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

void foreach(struct LinkedList *list, void (*function)(int64_t)) {
    while (list != NULL) {
        function(list->value);
        list = list->next;
    }
}

struct LinkedList *map(struct LinkedList *origin, int64_t (*function)(int64_t)) {
    struct LinkedList *head = NULL;
    while (origin != NULL) {
        list_add_front(&head, function(origin->value));
        origin = origin->next;
    }
    return head;
}

void map_mut(struct LinkedList **origin, int64_t (*func)(int64_t)) {
    if ((*origin) == NULL)
        return;
    (*origin)->value = func((*origin)->value);
    map_mut(&(*origin)->next, func);
    return;
}

int64_t foldl(int64_t accum, struct LinkedList *list, int64_t (*func)(int64_t, int64_t)) {
    while (list != NULL) {
        accum = func(list->value, accum);
        list = list->next;
    }
    return accum;
}

struct LinkedList *iterate(int64_t s, size_t n, int64_t (*func)(int64_t)) {
    struct LinkedList *list = list_create(s);
    n--;
    for (unsigned int i = 0; i < n; i++) {
        s = func(s);
        list_add_back(&list, s);
    }
    return list;
}


bool save(struct LinkedList *list, const char *filename) {
    FILE *file;
    file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }
    while (list != NULL) {
        fprintf(file, "%"PRId64" ", list->value);
        list = list->next;
    }
    fclose(file);
    return true;
}

bool load(struct LinkedList **list, const char *filename) {
    FILE *file;
    int64_t container;
    if ((file = fopen(filename, "r")) == NULL) {
        return false;
    }
    while (fscanf(file, "%"PRId64" ", &container) != EOF) {
        list_add_back(list, container);
    }
    fclose(file);
    return true;
}

bool serialize(struct LinkedList *list, const char *filename) {
    FILE *bin;
    if ((bin = fopen(filename, "wb")) == NULL) {
        return false;
    }
    while (list != NULL) {
        fwrite(&(list->value), sizeof(int64_t), 1, bin);
        list = list->next;
    }
    fclose(bin);
    return true;
}

bool deserialize(struct LinkedList **list, const char *filename) {
    FILE *bin;
    int64_t container;
    if ((bin = fopen(filename, "rb")) == NULL) {
        return false;
    }
    while (fread(&container, sizeof(int64_t), 1, bin) != 0) {
        list_add_back(list, container);
    }
    fclose(bin);
    return true;
}
