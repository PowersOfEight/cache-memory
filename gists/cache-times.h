#ifndef CACHE_TIMES_H
#define CACHE_TIMES_H

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SPINNER_DELAY 10000000L
#define COLLECTION_DELAY 100000L
#define COLLECTION_N_ITER 50000000L

#define DEFAULT_FILE_NAME "data/cache-data.csv"

#define BILLION 1000000000L
#define MAX_BUFFER 1024 * 1024

extern int iters;
extern volatile int spin;
extern volatile char dummy_char;


void *spinner();
void collect_data(size_t buffer_size, size_t stride, FILE *out);
int get_access_times();

#endif
