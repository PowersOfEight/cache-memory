#include "vector.h"


vector* init_vec(size_t initial_capacity){
    vector* vec = (vector*)malloc(sizeof(vector));
    vec->arr = (size_t*)malloc(sizeof(size_t)*initial_capacity);
    vec->capacity = initial_capacity;
    vec->count = 0;
    return vec;
}


void append(vector* vec, size_t value) {
    if (vec->count == vec->capacity) {
        vec->capacity *= 2;
        vec->arr = 
            (size_t*)realloc(vec->arr, 
                vec->capacity * sizeof(size_t));
    }
    vec->arr[vec->count++] = value;
}

void destroy_vec(vector* vec) {
    free(vec->arr);
    free(vec);
}

size_t get(vector* vec, size_t index) {
    if (index < 0 || index >= vec->capacity) {
        return -1;
    }
    return vec->arr[index];
}