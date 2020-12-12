#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>
#include "./utils.h"

#include "linked_list.h"
#include "higher_order_functions.h"

void print_space(int64_t i) { printf("%lld ", i);}
void print_newline(int64_t i) { printf("%lld\n", i);}
int64_t cube(int64_t v) { return v*v*v; }
int64_t times2(int64_t v) { return v*2; }
int64_t aaa(int64_t v, int64_t a) { return v + a; }

void file_bin_test(Node **iter) {
    puts("Process serialization...");
    if (!serialize(*iter, "./serialization.bin")) {
        puts("Serialization error");
    }
    puts("Serialization complete.");

    list_free(*iter);
    *iter = NULL;

    puts("Process deserialization...");
    if (!deserialize(iter, "./serialization.bin")) {
        puts("Deserialization error.");
    }

    printf("Deserialization complete, your list is: ");
    foreach(*iter, print_space);
    puts("");
}

int main() {
//    int64_t element;
//    Node* list;
//    if (scanf("%lld", &element) != EOF) {
//        list = list_create(element);
//        while (scanf("%lld", &element) != EOF) {
//            list_add_front(element, &list);
//        }
//    }
//    foreach(list, print_space); //yes
//    map(list, cube); // yes
//    map_mut(&list, imaxabs); // yes
//
//    size_t len = list_length(list);
//    int64_t sum = list_sum(list);
//    printf("length = %lu, sum = %"PRId64"\n", len, sum);
//    Node* second_node = list_get_at(2, list);
//    int64_t second_val = list_get(2, list);
//
//    if ((second_val != 0) && (second_node != NULL)) {
//        printf("list_get(2) = %"PRId64", list_node_at(2) = %"PRId64"\n",
//               second_val,
//               second_node->value);
//    } else {
//        printf("Error in list_node_at or list_get: element does not exist.\n");
//    }
//
//    int64_t su = foldl(0, aaa, list);
//    printf("foldl sum is: %"PRId64"\n", su);

    Node *iter = iterate(1, 10, times2);
    printf("Iteration completed: ");
    foreach(iter, print_space);
    printf("\n");

    if (!save(iter, "aaaa")) {
        printf("Save error.\n");
    }
    printf("Save completed.\n");
    list_free(iter);
    iter = NULL;

    if (!load(&iter, "aaaa"))
        printf("Load error.\n");
    printf("Load is completed, your list is: ");
    foreach(iter, print_space);
    printf("\n");


    file_bin_test(&iter);

//    if (!serialize(iter, "./serialization.bin"))
//        printf("Serialization error.\n");
//    printf("Serialization complete.\n");
//    list_free(&(*iter));
//    iter = NULL;
//
//    if (!deserialize(&iter, "./serialization.bin")) {
//        printf("Deserialization error.\n");
//    }
//    printf("Deserialization is completed, your list is: \n");
//    foreach(iter, print_space);
//    printf("\n");

    return 0;
}
