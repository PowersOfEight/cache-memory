#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../linked_list/linked_list.h"
#define REPEATS 50
volatile int dummy = 0;

typedef struct {
    long buffer_size;
    long double mean;
} measurement;

typedef struct {
    long high_size;
    long low_size;
    long double delta;
} delta;

int compare_deltas(const void* a, const void* b) {
    delta* left = (delta*) a;
    delta* right = (delta*) b;
    if (left->delta < right->delta) return 1;
    if (left->delta > right->delta) return -1;
    return 0;
}
/**
 * Consumes measurements and returns deltas
 */
delta deltas_from_measurments(linked_list* measurements) {
    delta results[measurements->length - 1];
    int i = 0;
    reset_front(measurements);
    measurement *curr, *nxt;
    while((curr = (measurement*)get_curr(measurements))!= NULL && 
       (unsigned long) i < measurements->length - 1) {
        next(measurements);
        nxt = (measurement*)get_curr(measurements);
        results[i].high_size = nxt->buffer_size;
        results[i].low_size = curr->buffer_size;
        results[i].delta = nxt->mean - curr->mean;
        i++;
    }
    qsort(results, measurements->length - 1 , sizeof(delta), compare_deltas);
    destroy_linked_list(measurements);
    return results[0];
}

void measure_cache_size() {
    /**
     * Ideas: Create a buffer and gradually increase it in size
     * Since we know the block size of the cache, we know that
     * the block size must divide the total cache size.
     */
    linked_list* measurements = init_linked_list(NULL);
    for (size_t size = 1024; size <= 1024 * 1024 * 64 * sizeof(int); size *= 2) {
        int* data = (int*)malloc(size * sizeof(int)); 
        measurement* measure = (measurement*)malloc(sizeof(measurement));
        struct timespec start, end;
        
        int stride = 32;
        // Warm up the cache
        for (size_t i = 0; i < size; i++) {
            data[i] = i;
        }

        // Start the clock
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int j = 0; j < REPEATS; j++) {
            for (size_t i = 0; i < size; i += stride) {
                dummy += data[i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // do something with dummy
        if (dummy < 0) {
            printf("%d\r", dummy);// literally prints the number then erases it
        }

        unsigned long elapsed = elapsed_time_ns(start, end);
        long double mean = (double) elapsed / (size * REPEATS / stride);
        measure->buffer_size = size * sizeof(int);
        measure->mean = mean;
        append_list(measurements, measure);
        printf("For an array of size %12lu KiB, mean access was %8.2Lf ns\n", size * sizeof(int) / 1024, mean);
        free(data);
    }
    delta max_delta = deltas_from_measurments(measurements);
    printf("The cache size is between %10lu KiB and %10lu KiB\n", max_delta.low_size/1024, max_delta.high_size/1024);
}

int main() {
    measure_cache_size();
    return 0;
}