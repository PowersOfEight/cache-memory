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

#define ARRAY_SIZE 1024 * 1024 * 32     // 16 MiB (2^{24}) to make sure we're larger than the cache
#define STEP_INCREASE 1024              // 1 KiB increments (2^{10})
#define ITERATIONS 100000000
#define DUMMY_PATH "data/dummy.txt"
#define MAX_MSG_LEN 1024
#define BILLION 1000000000L
#define SPINNER_DELAY 100000000L

// Some globals to get us started
volatile char dummy_char = (char) 0;
volatile int spin = 1;

size_t collect_data(size_t);

typedef struct {
    const char* file_path;
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
void exit_with_err_msg(const char* msg_format, ...) {
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
void assign_thread_to_core(pthread_t thread, int core_id) {
    cpu_set_t cpu_set;//    
    CPU_ZERO(&cpu_set); // Clears set so that it ccontains no CPUs
    CPU_SET(core_id, &cpu_set); // Adds CPU #<core_id> to the CPU set

    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpu_set) != 0) {
        exit_with_err_msg("Could not set affinity for thread %lu to CPU", thread);
    }
}


/**
 * Sets the scheduling for the thread described by `thread` to
 * real-time (or as close as we can get to it)
 * @arg `thread1: `pthread_t` -> The identifier of the thread
 */
void set_realtime_priority(pthread_t thread) {
    struct sched_param param;
    param.sched_priority = 99;

    if (pthread_setschedparam(thread, SCHED_FIFO, &param) != 0) {
        exit_with_err_msg("Could not set thread %lu to FIFO with priority %lu", thread, param);
    }
}

// Need to find the highest \Delta t for a run
void* data_thread(void* arg) {
    data_arg* data = (data_arg*) arg;
    set_realtime_priority(pthread_self());
    assign_thread_to_core(pthread_self(), 0);
    // Under Construction
    return NULL;
}

/**
 * Find the buffer size where \Delta t is maximum
 * \Delta t is t_avg[i] / t_avg[i - 1].  In other words
 * it scales with the size of t_avg.  \Delta t should be in
 * the range of [0, 100]. 
 */
size_t find_highest_delta_t_buff() {
    vector* my_vector = init_vec(1024);
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        append(my_vector, i);
    }
    for(size_t i = 0; i < my_vector->count; i++) {
        printf("my_vec[%lu] = %lu\n", i, get(my_vector, i));
    }
    destroy_vec(my_vector);
}



size_t collect_data(size_t buffer_size) {
    char *buffer = malloc(buffer_size);
    
    free(buffer);
}



int main(int argc, char **argv)
{
    // pthread_t data_thread_id, spinner_thread;
    // data_arg datarg = {.file_path = "data/test.csv"};
    // printf("Collecting data...\n");
    // pthread_create(&spinner_thread, NULL, spinner, NULL);
    // pthread_create(&data_thread_id, NULL, data_thread, (void*)&datarg);
    // pthread_join(data_thread_id, NULL);
    // spin = 0;
    // pthread_join(spinner_thread, NULL);
    find_highest_delta_t_buff();

    return 0;
}