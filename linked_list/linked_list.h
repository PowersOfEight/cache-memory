
typedef struct list_node{
     void* object;
     struct list_node* next;
     struct list_node* prev;
} list_node;

typedef struct{
    list_node* head;
    list_node* tail;
    unsigned long length;
} linked_list;

/**
 * Creates a new doubly-linked
 * list and returns its pointer
 */
linked_list* init_linked_list();
/**
 * Appends `obj` to the tail `list`
 */
unsigned long append(linked_list* list, void* obj);

/**
 * Destroys `list`
 */
void destroy_linked_list(linked_list* list);