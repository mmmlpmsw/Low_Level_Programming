#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "linked_list.h"
#include "higher_order_functions.h"

void foreach(Node* head, void (*func) (int64_t)) {
    while (head != NULL) {
        func(head->value);
        head = head->next;
    }
}

Node* map(Node* source, int64_t (*func) (int64_t)) {
    Node* new_list;
    while (source != NULL) {
        list_add_front(func(source->value), &new_list);
        source = source->next;
    }
    return new_list;
}



void map_mut(Node** origin, int64_t (*func)(int64_t)) {
    if ((*origin) == NULL)
        return;
    (*origin)->value = func((*origin)->value);
    map_mut(&(*origin)->next, func);
    return;
}


int64_t foldl(int64_t accumulator, int64_t (*func) (int64_t, int64_t), Node * head) {
    while (head != NULL) {
        accumulator = func(head->value, accumulator);
        head = head->next;
    }
    return accumulator;
}

Node* iterate(int64_t value, size_t size, int64_t (*func)(int64_t)) {
    Node* list = list_create(value);
    size--;
    for (unsigned int i = 0; i < size; i++) {
        value = func(value);
        list_add_back(value, &list);
    }
    return list;
}