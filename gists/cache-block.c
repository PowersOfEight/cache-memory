#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../linked_list/linked_list.h"
#include "../utils/utils.h"

#define ARRAY_SIZE 1024 * 1024 // A large enough array to exceed typical cache sizes
#define REPEATS 1000           // Number of iterations for averaging
#define THRESHOLD 3
#define BILLION 1e9L

// if we do this over and over, the mode of all spikes should point us in the 
// direction of the 
void measure_cache_block_size()
{
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    int block_size = 0;
    double times[11];
    int steps[11];
    int count = 0;
    for (int step = 16; step <= 1024; step *= 2)
    { // Increase step size
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int repeat = 0; repeat < REPEATS; repeat++)
        {
            for (int i = 0; i < ARRAY_SIZE; i += step)
            {
                array[i] += 1; // Access the array with a specific stride
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        unsigned long n = REPEATS * ARRAY_SIZE / step;
        unsigned long time_taken = elapsed_time_ns(start, end);
        double mean_time = (double)time_taken / n;
        printf("Step size: %d, Time: %8f nano-seconds\n", step, mean_time);
        // Look for a jump in time to determine block size
        if (block_size == 0 && mean_time > THRESHOLD)
        {
            block_size = step;
        }
        steps[count] = step;
        times[count] = mean_time;
        count++;
    }
    // find the first "spike"
    linked_list *spikes = init_linked_list(NULL);
    for (int i = 1; i < count - 1; ++i)
    {
        double slope_in = times[i] - times[i - 1];
        double slope_out = times[i + 1] - times[i];
        if (slope_in > 0 && slope_out < 0)
        {
            int *step = (int *)malloc(sizeof(int));
            *step = steps[i];
            append_list(spikes, step);
        }
    }
    reset_front(spikes);
    int *spike;
    while ((spike = get_curr(spikes))!= NULL) {
        printf("Spike detected at step size %d bytes\n", *spike);
        if (!next(spikes)) break;
    }
    destroy_linked_list(spikes);
    free(array);
    printf("Estimated cache block size: %d bytes\n", block_size);
}

int main()
{
    measure_cache_block_size();
    return 0;
}