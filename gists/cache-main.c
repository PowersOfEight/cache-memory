#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list/linked_list.h"
#include "cache-block.h"
#include "cache-size.h"
#define N 100
#define SPINNER_DELAY 100000000L

volatile int spin = 1;
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
            delay_count = 0;
            printf("\r%c", spinner_chars[ndx]);
            fflush(stdout);
            ndx = (ndx + 1) % num_chars;
            delay_count = 0;
        }
    }
    printf("\r \n");
    return NULL;
}

int find_mode_int(linked_list* list) {
    int max_count = 0;
    if (list->length == 0) return max_count;
    reset_front(list);
    int mode = *(int*)get_curr(list);
    int* curr;
    while ((curr = get_curr(list)) != NULL) {
        int count = 0;
        list_node* tmp = list->curr;
        while(tmp != list->tail) {
            if (*curr == *(int*)tmp->object){
                count++;
            }
            tmp = tmp->next;
        }
        if (count > max_count) {
            max_count = count;
            mode = *curr;
        }
        if(!next(list)) break;
    }
    return mode;
}

int main(){
    pthread_t spinner_thread;
    spin = 1;
    linked_list* int_block_list = init_linked_list(NULL);
    printf("Collecting data...\n");
    pthread_create(&spinner_thread, NULL, spinner, NULL);
    for(int i = 0; i < N; i++) {
        linked_list *block_sizes = measure_cache_block_size();
        reset_front(block_sizes);
        int* curr;
        while((curr = get_curr(block_sizes)) != NULL) {
            int* val = (int*)malloc(sizeof(int));
            (*val) = (*curr);
            append_list(int_block_list, val);
            if(!next(block_sizes)) break;
        }
        destroy_linked_list(block_sizes);
    }
    int block_size = find_mode_int(int_block_list);
    spin = 0;
    pthread_join(spinner_thread, NULL);
    printf("Estimated block size: %d\n", block_size);
    delta measurement = measure_cache_size();
    destroy_linked_list(int_block_list);
    return 0;
}