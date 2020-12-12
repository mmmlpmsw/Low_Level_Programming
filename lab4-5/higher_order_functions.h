#include "linked_list.h"
#include <stdio.h>

#ifndef LAB4_HIGHER_ORDER_FUNCTIONS_H
#define LAB4_HIGHER_ORDER_FUNCTIONS_H

void foreach(Node*, void (*func) (int64_t));
Node* map(Node*, int64_t (*func) (int64_t));
void map_mut(Node**, int64_t (*func) (int64_t));
int64_t foldl(int64_t, int64_t (*func) (int64_t, int64_t), Node *);
Node* iterate(int64_t value, size_t size, int64_t (*func)(int64_t));
#endif