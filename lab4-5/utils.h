#include <stdbool.h>
#include "linked_list.h"
#include "higher_order_functions.h"
#ifndef LAB4_UTILS_H
#define LAB4_UTILS_H

bool save(Node*, const char*);
bool load(Node**, const char*);
bool serialize(Node*, const char*);
bool deserialize(Node**, const char*);

#endif
