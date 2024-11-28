#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"


vector* init_vec(
    unsigned long initial_capacity, 
    unsigned long size, 
    void (*destroy_obj)){
    if (size < 1 || initial_capacity < 1) {
        perror("Vector must have positive initial capacity and size");
        exit(EXIT_FAILURE);
    }
    vector* vec = (vector*)malloc(sizeof(vector));
    vec->size = size;
    vec->capacity = initial_capacity > 0 ? initial_capacity:1;
    vec->count = 0;
    vec->destroy_obj = destroy_obj;
    vec->arr = (void*)malloc(vec->size*vec->capacity);
    return vec;
}

static void expand(vector* vec) {
    if (!vec) return;
    printf("Expanding vector: current capacity %lu\n", vec->capacity);
    vec->capacity *= 2;
    vec->arr = realloc(vec->arr, vec->size * vec->capacity);
}

void append(vector* vec, void* value) {
    if (vec->count == vec->capacity) {
        expand(vec);
    }
    void* target = (char*)vec->arr + (vec->count * vec->size);
    memcpy(target, value, vec->size);
    vec->count++;
}

void destroy_vec(vector* vec) {
    if (!vec) return;

    for(unsigned long i=0; i < vec->count; ++i) {
        void* elem = (char*)vec->arr + (i * vec->size);
        if (vec->destroy_obj) {
            vec->destroy_obj(elem);
        }
    }
    free(vec->arr);
    free(vec);
}

void* get(vector* vec, unsigned long index) {
    if (index >= vec->count) {
        return NULL;
    }
    return (void*)((char*)vec->arr + (index * vec->size));
}

