#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../utils/utils.h"

volatile char dummy = (char) 0;

void measure_cache_size() {
    /**
     * Ideas: Create a buffer and gradually increase it in size
     * Since we know the block size of the cache, we know that
     * the block size must divide the total cache size.
     */
    for (size_t size = 1024; size <= 1024 * 1024 * 128; size *= 2) {
        char* bytes = (char*)malloc(size); 
        struct timespec start, end;
        
        // Warm up the cache
        for (size_t i = 0; i < size; i++) {
            bytes[i] = (char) i;
        }

        // Start the clock
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t i = 0; i < size; i++) {
            dummy += bytes[i];
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // do something with dummy
        if (dummy == 'a') {
            printf("%c\r", dummy);// literally prints the character 'a' then erases it
        }

        unsigned long elapsed = elapsed_time_ns(start, end);
        long double mean = (double) elapsed / size;

        printf("For an array of size %12lu KiB, mean access was %8.2Lf\n", size / 1024, mean);
        free(bytes);
    }
}

int main() {
    measure_cache_size();
    return 0;
}