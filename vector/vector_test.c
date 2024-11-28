#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

void destroy_string(void* obj) {
    printf("Destroying string \"%s\"\n", (char*)obj);
    free(obj);
}

typedef struct {
    unsigned long stride;
    unsigned long size;
    unsigned long *arr;
} record;

record* generate_record(unsigned long stride, unsigned long size) {
    record* result = (record*)malloc(sizeof(record));
    result->size = size;
    result->stride = stride;
    result->arr = (unsigned long*)malloc(sizeof(unsigned long) * (size / stride));
    for (int i = 0; i < size / stride; i++) {
        result->arr[i] = i + 1;
    }
    return result;
}

void destroy_record(void* rec) {
    record* ptr = (record*) rec;
    printf("Destroying record with size %lu and stride %lu\n", ptr->size, ptr->stride);
    free(ptr->arr);
    free(rec);
}



int main(int argc, char** argv) {
    vector* vec = init_vec(1, sizeof(record), destroy_record);

    for(int size = 1024; size < 1024 * 1024; size *= 2) {
        for(int stride = 1; stride < 64; stride *= 2) {
            append(vec, generate_record(stride, size));
        }
    }

    destroy_vec(vec);
    return 0;
}