/**
 * My attempt at a dynamic C array
 */
typedef struct {
    void *arr;
    unsigned long  capacity;
    unsigned long  count;
    unsigned long  size;
    void (*destroy_obj)(void*);
} vector;

/**
 * Initializes this vector with
 * capacity given by thits argument
 */
vector* init_vec(unsigned long initial_capacity, unsigned long size, void (*destroy_obj));
/**
 * Adds value to vec
 */
void append(vector* vec, void *value);
/**
 * Destroys the vector and frees
 * all of its memory
 */
void destroy_vec(vector* vec);
/**
 * Gets the element at index 
 */
void* get(vector* vec, unsigned long index);





