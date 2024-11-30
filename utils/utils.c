#include <time.h>
#include "utils.h"

unsigned long elapsed_time_ns(struct timespec start, struct timespec end) {
    unsigned long elapsed = 0;
    if (end.tv_nsec < start.tv_nsec) {
        elapsed = ((end.tv_sec - start.tv_sec - 1) * 1e9) + (end.tv_nsec + 1e9 - start.tv_nsec);
    } else {
        elapsed = ((end.tv_sec - start.tv_sec) * 1e9) + (end.tv_nsec - start.tv_nsec);
    }
    return elapsed;
}