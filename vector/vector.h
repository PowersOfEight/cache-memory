#include <stdlib.h>


/**
 * My attempt at a dynamic C array
 */
typedef struct {
    size_t *arr;
    size_t  capacity;
    size_t  count;
} vector;

/**
 * Initializes this vector with
 * capacity given by thits argument
 */
vector* init_vec(size_t initial_capacity);
/**
 * Adds value to vec
 */
void append(vector* vec, size_t value);
/**
 * Destroys the vector and frees
 * all of its memory
 */
void destroy_vec(vector* vec);
/**
 * Gets the element at index 
 */
size_t get(vector* vec, size_t index);





