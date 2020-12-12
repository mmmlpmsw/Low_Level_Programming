#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "utils.h"

bool save(Node* list, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL)
        return false;
    while (list != NULL) {
        fprintf(file, "%"PRId64" ", list->value);
        list = list->next;
    }
    fclose(file);
    return true;
}

bool load(Node** list, const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }
    int64_t val;
    while (fscanf(file, "%"PRId64" ", &val) != EOF) {
        list_add_front(val, list);
    }
    fclose(file);
    return true;
}

bool serialize(struct Node *list, const char *filename) {
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

bool deserialize(struct Node **list, const char *filename) {
    FILE *bin;
    int64_t container;
    if ((bin = fopen(filename, "rb")) == NULL) {
        return false;
    }
    while (fread(&container, sizeof(int64_t), 1, bin) != 0) {
        list_add_back(container, list);
    }
    fclose(bin);
    return true;
}