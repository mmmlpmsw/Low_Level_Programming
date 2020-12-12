#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

Node* list_create(int64_t value) {
    Node * new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

void list_add_front(int64_t value, Node** head) {
    Node* new_node = malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = *head;
    *head = new_node;
}

void list_add_back(int64_t value, Node** head) {
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        new_node->value = value;
        return;
    }
    Node* current_element = *head;
    while (current_element->next != NULL)
        current_element = current_element->next;
    current_element->next = new_node;
    new_node->value = value;
    new_node->next = NULL;
}

int64_t list_get(int64_t index, Node* head) {
    Node* res = list_get_at(index, head);
    if (res)
        return res->value;
    else return 0;
}

void list_free(Node* head) {
    if (head == NULL)
        return;
    list_free(head->next);
    free(head);
}

int64_t list_length(Node* head) {
    if (head == NULL)
        return 0;
    int64_t count = 1;
    while (head->next != NULL) {
        head = head->next;
        count ++;
    }
    return count;
}

Node* list_get_at(int64_t index, Node* head) {
    if (index < 0)
        return 0;
    if (head == NULL)
        return 0;
    for (int64_t i = index; i > 0; i --) {
        if (head->next == NULL)
            return 0;
        head = head->next;
    }
    return head;
}

int64_t list_sum(Node* head) {
    int64_t sum = 0;
    while (head->next != NULL) {
        sum += head->value;
        head = head->next;
    }
    sum += head->value;
    return sum;
}