
typedef struct list_node{
     void* object;
     struct list_node* next;
     struct list_node* prev;
} list_node;

typedef struct{
    list_node* head;
    list_node* tail;
    list_node* curr;
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
 * Resets the current node of list
 * to the beginning
 */
void reset_front(linked_list* list);

/**
 * Resets the current node of the list
 * to the end
 */
void reset_back(linked_list* list);

/**
 * Advances to the next item in the list
 * if there is one. 
 * @return 1 If advance was successful, 0 otherwise
 */
int next(linked_list* list);

/**
 * Moves to the previous item in the list
 * if there is one
 * @return 1 if previous was successful, 0 otherwise
 */
int previous(linked_list* list);

/**
 * Appends `obj` to the tail `list`
 * @return The new length of the list
 */
unsigned long append_list(linked_list* list, void* obj);

/**
 * Returns a pointer to the current object.
 * @return Pointer to the current object
 */
void* get_curr(linked_list* list);

/**
 * 
 */


/**
 * Destroys `list`
 */
void destroy_linked_list(linked_list* list);