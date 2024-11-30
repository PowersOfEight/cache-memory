#include <stdio.h>
#include <stdlib.h>
#include "linked_list/linked_list.h"
#include "cache-block.h"
#include "cache-size.h"

#define N = 100

int main(){

    linked_list *block_sizes = measure_cache_block_size();
    delta measurement = measure_cache_size();
    destroy_linked_list(block_sizes);
    return 0;
}