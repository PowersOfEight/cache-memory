#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

# define SPINNER_DELAY 100000000
# define COLLECTION_DELAY 100000
# define COLLECTION_N_ITER 50000000

volatile int spin = 1;

typedef struct {
    int value;
    pthread_mutex_t lock;
} atomic_t;

int read_atom(atomic_t* atom) {
    int result;
    pthread_mutex_lock(&atom->lock);
    result = atom->value;
    pthread_mutex_unlock(&atom->lock);
    return result;
}

int set_atom(atomic_t* atom, int value) {
   int result;
   pthread_mutex_lock(&atom->lock);
   result = atom->value;
   atom->value = value;
   pthread_mutex_unlock(&atom->lock);
   return result; 
}

// return 1
int init_atomic_t(atomic_t *atom, int value) {
    int result = 0;
    result += pthread_mutex_init(&atom->lock, NULL);
    atom->value = value;
    return result;
}

// void measure_access_time(size_t buffer_size, size_t stride);



void *spinner() {
    const char spinner_chars[] = {'|', '/', '-', '\\'};
    const int num_chars = 4;
    int ndx = 0;
    // atomic_t* flag = (atomic_t*) args;
    struct timespec interval;
    
    interval.tv_sec = 0;
    interval.tv_nsec = SPINNER_DELAY;
    while (spin == 1) {
        printf("\r%c\t", spinner_chars[ndx]);
        fflush(stdout);
        ndx = (ndx + 1) % num_chars;
        nanosleep(&interval, NULL);// Delays animation
        // usleep(SPINNER_DELAY);
    }

    printf("\rAnimation complete!\n");
    return NULL;
}

void collect_data() {
    FILE* out;
    out = fopen("./bin/debug", "w+");
    // struct timespec interval;
    size_t i;

    // interval.tv_nsec = COLLECTION_DELAY;
    // this just spins for now
    for ( i = 0; i < COLLECTION_N_ITER; ++i) {
        fprintf(out, "\r%ld", i);
        // fflush(out);
        // nanosleep(&interval, NULL);
    }

    spin=0;
    fclose(out);
}

int main(int argc, char** argv) {

    pthread_t spinner_thread;
    // atomic_t spinner_flag;// Tells the spinner thread to keep spinning;
    // if (init_atomic_t(&spinner_flag, 1) != 0) {
    //     perror("Failed to initialize spinner flag!");
    //     return EXIT_FAILURE;
    // } 


    if (pthread_create(&spinner_thread, NULL, spinner, NULL) != 0) {
        perror("Thread creation failure!");
        return EXIT_FAILURE;
    } else {
        
    }

    collect_data();// Simulates data collection for now


    if (pthread_join(spinner_thread, NULL) != 0) {
        perror("Failed to join spinner thread");
        return EXIT_FAILURE;
    }

    printf("Test Successful\n");
    return 0;
}