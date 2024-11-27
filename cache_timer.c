#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include "vector/vector.h"

#define ARRAY_SIZE 1024 * 1024 * 64 // 16 MiB (2^{24}) to make sure we're larger than the cache
#define STEP_INCREASE 1024          // 1 KiB increments (2^{10})
#define ITERATIONS 100000000
#define DUMMY_PATH "data/dummy.txt"
#define MAX_MSG_LEN 1024
#define BILLION 1000000000L
#define SPINNER_DELAY 100000000L
#define MAX_FACTOR 150
#define MAX_LONG 18446744073709551615L

// Some globals to get us started
char dummy_char = (char)0;
volatile int spin = 1;

vector *collect_data(size_t);

typedef struct
{
    const char *file_path;
} data_arg;

void *spinner()
{
    const char spinner_chars[] = {'|', '/', '-', '\\'};
    const int num_chars = 4;
    int ndx = 0;
    int delay_count = 0;

    while (spin == 1)
    {
        if (++delay_count >= SPINNER_DELAY)
        {
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
/**
 * Prints error message using `msg_format` and args
 * using `perror`, then exits
 * @arg `msg_format` : `const char*` Format string
 * @arg ... : parameters for format
 */
void exit_with_err_msg(const char *msg_format, ...)
{
    va_list args;
    char error_msg[MAX_MSG_LEN];
    va_start(args, msg_format);
    vsprintf(error_msg, msg_format, args);
    va_end(args);
    perror(error_msg);
    exit(EXIT_FAILURE);
}

/**
 * Assigns thread described by `thread` to the core described by
 * `core_id`
 * @arg `thread`: `pthread_t` -> The identifier of the thread being assigned
 * @arg `core_id`: `int` -> The ID of the core to assign `thread` to
 */
void assign_thread_to_core(pthread_t thread, int core_id)
{
    cpu_set_t cpu_set;          //
    CPU_ZERO(&cpu_set);         // Clears set so that it ccontains no CPUs
    CPU_SET(core_id, &cpu_set); // Adds CPU #<core_id> to the CPU set

    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpu_set) != 0)
    {
        exit_with_err_msg("Could not set affinity for thread %lu to CPU", thread);
    }
}

/**
 * Sets the scheduling for the thread described by `thread` to
 * real-time (or as close as we can get to it)
 * @arg `thread1: `pthread_t` -> The identifier of the thread
 */
void set_realtime_priority(pthread_t thread)
{
    struct sched_param param;
    param.sched_priority = 99;

    if (pthread_setschedparam(thread, SCHED_FIFO, &param) != 0)
    {
        exit_with_err_msg("Could not set thread %lu to FIFO with priority %lu", thread, param);
    }
}

// Need to find the highest \Delta t for a run
void *data_thread(void *arg)
{
    // data_arg *data = (data_arg *)arg;
    set_realtime_priority(pthread_self());
    assign_thread_to_core(pthread_self(), 0);
    // Under Construction
    return collect_data(ARRAY_SIZE);
}

/**
 * Find the buffer size where \Delta t is maximum
 * \Delta t is t_avg[i] / t_avg[i - 1].  In other words
 * it scales with the size of t_avg.  \Delta t should be in
 * the range of [0, 100].
 */
size_t find_highest_delta_t_buff(vector* times)
{
    size_t result = 0;
    if (times->count >= 2) {
        for(size_t i = 1; i < times->count; i++) {
            size_t curr = get(times, i);
            size_t last = get(times, i-1);
            if (curr > last) {
                size_t delta = curr - last;
                if (delta > result) {
                    result = delta;
                }
            }
        }
    }
    return result;
}

int compare(const void *a, const void* b) {
    const size_t* left = (const size_t*) a;
    const size_t* right = (const size_t*)b;
    if ((*left) > (*right)) {
        return 1;
    } else if ((*right) > (*left)) {
        return -1;
    } else {
        return 0;
    }
}

size_t find_median_difference(vector* times) {
    vector* jumps = init_vec(times->count);
    size_t median = 0;
    for (size_t i = 1; i < times->count; i++) {
        size_t curr = get(times, i);
        size_t last = get(times, i-1);
        if (curr > last) {
            append(jumps, curr - last);
        }
    }
    qsort(jumps->arr, jumps->count, sizeof(size_t), compare);
    if ((jumps->count & 1) == 0) {// even
        median = (get(jumps, (jumps->count/2) - 1) + get(jumps, (jumps->count/2)))/2;
    } else {
        median = get(jumps, jumps->count/2);
    }
    destroy_vec(jumps);
    return median;
}



vector *collect_data(size_t buffer_size)
{
    char *buffer = malloc(buffer_size);
    FILE *dummy_file = fopen64(DUMMY_PATH, "w+");
    vector* results = init_vec(1024);
    size_t min = MAX_FACTOR;
    if (!dummy_file)
    {
        exit_with_err_msg("Could not open file at path %s\n", DUMMY_PATH);
    }

    struct timespec start, end;
    // warm up the cache
    for (size_t i = 0; i < buffer_size; i++)
    {
        buffer[i] = (char)i;
    }
    for (size_t i = 0; i < buffer_size; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        dummy_char += (char)buffer[i];
        clock_gettime(CLOCK_MONOTONIC, &end);
        size_t diff =
            ((end.tv_sec - start.tv_sec) * BILLION) + (end.tv_nsec - start.tv_nsec);
        if (diff < min) {
            min = diff;
        } 
        if (diff <= (min * MAX_FACTOR)) {// Filters outlandishly high values
            append(results, diff);
        }
    }
    fprintf(dummy_file, "dummy_char: %d\n", (int)dummy_char);
    fclose(dummy_file);
    free(buffer);
    return results;
}

int main(int argc, char **argv)
{
    pthread_t data_thread_id, spinner_thread;
    data_arg datarg = {.file_path = "data/test.csv"};
    vector* results;
    printf("Collecting data...\n");
    pthread_create(&spinner_thread, NULL, spinner, NULL);
    pthread_create(&data_thread_id, NULL, data_thread, (void*)&datarg);
    pthread_join(data_thread_id,(void**) &results);
    spin = 0;
    pthread_join(spinner_thread, NULL);
    printf("Largest delta: %lu\n", find_highest_delta_t_buff(results));
    printf("Median jump: %lu\n", find_median_difference(results));
    destroy_vec(results);
    printf("Done...\n");
    return 0;
}