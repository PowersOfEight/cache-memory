#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../utils/utils.h"
#define REPEATS 100
volatile int dummy = 0;

void measure_cache_size() {
    /**
     * Ideas: Create a buffer and gradually increase it in size
     * Since we know the block size of the cache, we know that
     * the block size must divide the total cache size.
     */
    for (size_t size = 1024; size <= 1024 * 1024 * 64 * sizeof(int); size *= 2) {
        int* data = (int*)malloc(size * sizeof(int)); 
        struct timespec start, end;
        
        int stride = 16;
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

        printf("For an array of size %12lu KiB, mean access was %8.2Lf ns\n", size * sizeof(int) / 1024, mean);
        free(data);
    }
}

int main() {
    measure_cache_size();
    return 0;
}