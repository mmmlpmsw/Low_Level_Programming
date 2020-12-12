#include <stdint.h>
# ifndef linked_list
#define linked_list

struct Node {
    struct Node* next;
    int64_t value;
};

typedef struct Node Node;

Node* list_create(int64_t);
void list_add_front(int64_t, Node**);
void list_add_back(int64_t, Node**);
int64_t list_get(int64_t, Node*);
void list_free(Node*);
int64_t list_length(Node*);
Node* list_get_at(int64_t, Node*);
int64_t list_sum(Node*);

#endif