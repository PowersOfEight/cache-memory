#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ARRAY_SIZE 1024 * 1024 // A large enough array to exceed typical cache sizes
#define REPEATS 1000           // Number of iterations for averaging
#define THRESHOLD 3
#define BILLION 1e9L

void measure_cache_block_size() {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    int block_size = 0;
    for (int step = 1; step <= 1024; step *= 2) { // Increase step size
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int repeat = 0; repeat < REPEATS; repeat++) {
            for (int i = 0; i < ARRAY_SIZE; i += step) {
                array[i] += 1; // Access the array with a specific stride
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        unsigned long n = REPEATS * ARRAY_SIZE / step;
        long time_taken = ((end.tv_sec - start.tv_sec) * BILLION) + (end.tv_nsec - start.tv_nsec);
        double mean_time = (double) time_taken / n;
        printf("Step size: %d, Time: %5.2f nano-seconds\n", step, mean_time);

        // Look for a jump in time to determine block size
        if (block_size == 0 && mean_time > THRESHOLD) {
            block_size = step;
        }
    }

    free(array);
    printf("Estimated cache block size: %d bytes\n", block_size);
}

int main() {
    measure_cache_block_size();
    return 0;
}