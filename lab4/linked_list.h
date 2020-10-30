#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdint.h>

struct LinkedList {
    int64_t value;
    struct LinkedList *next;
};

struct LinkedList *list_create(int64_t first_val);
void list_add_front(struct LinkedList **head, int64_t value);
void list_add_back(struct LinkedList **head, int64_t value);
int64_t list_get_at(struct LinkedList *head, size_t index);
void list_free(struct LinkedList *head);
size_t list_length(struct LinkedList *head);
struct LinkedList *list_node_at(struct LinkedList *head, size_t index);
int64_t list_sum(struct LinkedList *head);

#endif
