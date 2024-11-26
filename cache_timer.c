#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#define ARRAY_SIZE 1024 * 1024 * 16     // 16 MiB (2^{24}) to make sure we're larger than the cache
#define STEP_INCREASE 1024              // 1 KiB increments (2^{10})
#define ITERATIONS 100000000


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
        const char* msg_format = "Could not set affinity for thread %lu to CPU";
        char error_msg[1024];
        sprintf(error_msg, msg_format, thread);
        perror(error_msg);
        exit(EXIT_FAILURE);
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
        const char* msg_format = "Could not set thread %lu to FIFO with priority %lu";
        char error_msg[1024];
        sprintf(error_msg, msg_format, thread, param);
        perror(error_msg);
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char **argv)
{
 
    return 0;
}