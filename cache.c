#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "cluster.h"


# define SPINNER_DELAY 200000000L
# define COLLECTION_DELAY 100000L
# define COLLECTION_N_ITER 50000000L

# define DEFAULT_FILE_NAME "data/cache-data.csv"

# define BILLION 1000000000L
# define MAX_BUFFER 32 * 1024 * 1024
# define N_CENTERS 2097152
# define MIN_STRIDE 64
// Let's record one center for each of the runs

volatile int spin = 1;
/**
 * Forces this out of optimization on reads
 */
volatile char dummy_char = (char)0;
size_t it_counter = 0;


void* spinner(void* args) {
    const char spinner_chars[] = { '|', '/', '-', '\\' };
    const int num_chars = 4;
    int ndx = 0;
    int delay_count = 0;

    while (spin == 1) {
        if (++delay_count >= SPINNER_DELAY) {
            delay_count = dummy_char;
            printf("\r%c", spinner_chars[ndx]);
            fflush(stdout);
            ndx = (ndx + 1) % num_chars;
            delay_count = 0;
        }

    }
    printf("\r \n");
    return NULL;
}

int compare(const void* a, const void* b) {
    return (*(size_t*)a - *(size_t*)b);
}

size_t median(size_t arr[], size_t n) {
    qsort(arr, n, sizeof(size_t), compare);

    if ((n & 1) == 0) { // even
        return (arr[n / 2 - 1] + arr[n / 2 ]) / 2;
    } else {
        return arr[n/2];
    }
}



// Now trying to see if we can get clustering to work.
// Success looks like gathering very Large clusters
cluster* collect_median_access_time(size_t buffer_size, size_t stride) {
    size_t iterations = buffer_size / stride;// Total iterations
    size_t* data = malloc(sizeof(size_t) * iterations);
    char* buffer = malloc(buffer_size);
    cluster* clusters = malloc(2 * sizeof(cluster));
    
    /**
     * May fail to allocate due to th volume of memory we're
     * allocating here.
     */
    if (!buffer || !data) {
        perror("Memory Allocation Failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    /**
     * The volatile keyword allows us to avoid compiler optimizations
     * for this value.
     */
     // volatile char dummy_char;

    for (size_t i = 0; i < iterations; i++) {
        buffer[i * stride] = (char)i;
    }

    for (size_t i = 0; i < iterations; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        dummy_char = buffer[i * stride];
        clock_gettime(CLOCK_MONOTONIC, &end);
        data[i] = (end.tv_sec - start.tv_sec) * BILLION + (end.tv_nsec - start.tv_nsec);
        it_counter++;
    }

    k_means(data, iterations, 2, clusters);
    free(buffer);
    free(data);
    return clusters;
}

int main(int argc, char** argv) {
    pthread_t spinner_thread;

    spin = 1;
    if (pthread_create(&spinner_thread, NULL, spinner, NULL) != 0) {
        perror("Thread creation failure!");
        return EXIT_FAILURE;
    }
    for (size_t buffer_size = 1024; buffer_size <= MAX_BUFFER; buffer_size *= 2) {
        for (size_t stride_size = 1; stride_size <= MIN_STRIDE; stride_size *= 2) {
            // collect_median_access_time(buffer_size, stride_size, outfile);
            cluster* clusters = collect_median_access_time(buffer_size, stride_size);
            printf("Center access time at mean %lu has %lu data points\n", 
                clusters[0].centroid, 
                clusters[0].point_count);
            printf("Center access time at mean %lu has %lu data points\n", 
                clusters[1].centroid, 
                clusters[1].point_count);
            free(clusters);
        }
    }
    spin = 0;
    if (pthread_join(spinner_thread, NULL) != 0) {
        perror("Failed to join spinner thread");
        return EXIT_FAILURE;
    }

    printf("Run complete\nTotal Iterations: %lu\n", it_counter);
    return 0;
}