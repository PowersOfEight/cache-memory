#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


# define SPINNER_DELAY 10000000L
# define COLLECTION_DELAY 100000L
# define COLLECTION_N_ITER 50000000L

# define DEFAULT_FILE_NAME "data/cache-data.csv"

# define BILLION 1000000000L
# define MAX_BUFFER = 32 * 1024 * 1024

volatile int spin = 1;
volatile char dummy_char = (char)0;
void* spinner() {
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
    return NULL;
    printf("\r\n");
}



// Every data point has some value associated with it
// Buffer Size, Stride (how far into the buffer we're accessing), time_diff
// Lets make a VERY big file and read these in as structs
void collect_data(size_t buffer_size, size_t stride, FILE* out) {
    char* buffer = malloc(buffer_size);

    /**
     * May fail to allocate due to th volume of memory we're
     * allocating here.
     */
    if (!buffer) {
        perror("Memory Allocation Failed");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;
    size_t iterations = buffer_size / stride;// Total iterations
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
        unsigned long diff = (end.tv_sec - start.tv_sec) * BILLION + (end.tv_nsec - start.tv_nsec);
        fprintf(out, "%lu,%lu,%lu\r\n", buffer_size, stride, diff);
    }

    free(buffer);
}

int main(int argc, char** argv) {
    const char* filename;
    pthread_t spinner_thread;
    FILE* outfile;
    const size_t stride = 64;
    size_t max_buffer = 32 * 1024 * 1024; // 2^25

    if (argc < 1) {
        filename = DEFAULT_FILE_NAME;
    }

    outfile = fopen(filename, "w");

    if (!outfile) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }


    for (size_t buffer_size = 1024; buffer_size <= max_buffer; buffer_size *= 2) {
        for (size_t stride_size = 1; stride_size <= stride; stride_size *= 2) {
            spin = 1;
            if (pthread_create(&spinner_thread, NULL, spinner, NULL) != 0) {
                perror("Thread creation failure!");
                return EXIT_FAILURE;
            }
            collect_data(buffer_size, stride, outfile);
            spin = 0;
            if (pthread_join(spinner_thread, NULL) != 0) {
                perror("Failed to join spinner thread");
                return EXIT_FAILURE;
            }
        }
    }

    fclose(outfile);
    printf("Run complete\n");
    return 0;
}