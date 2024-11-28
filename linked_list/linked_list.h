
typedef struct list_node{
     void* object;
     struct list_node* next;
     struct list_node* prev;
} list_node;

typedef struct{
    list_node* head;
    list_node* tail;
    unsigned long length;
    // optional destructor for object
    void (*destroy_obj)(void* obj);
} linked_list;

/**
 * Creates a new doubly-linked
 * list and returns its pointer. 
 * The optional `destroy_object` function
 * can be called to deallocate memory
 * when this linked list is destroyed.
 */
linked_list* init_linked_list(void (*destroy_object)(void*));
/**
 * Creates a new doubly-linked list
 * without the optional `destroy_object` function.
 * Note this will still call `free(obj)`
 */
linked_list* init_simple_linked_list();

/**
 * Appends `obj` to the tail `list`
 */
unsigned long append(linked_list* list, void* obj);

/**
 * Destroys `list`
 */
void destroy_linked_list(linked_list* list);