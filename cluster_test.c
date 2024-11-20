#include <stdio.h>
#include "cluster.h"

int main() {
    size_t dummy[] = {1, 56, 18, 75, 99, 36, 12, 34, 29, 76, 34, 56, 75, 34, 29, 14, 99, 32};

    cluster* clusters = malloc(2* sizeof(cluster));
    
    k_means(dummy, 18, 2, clusters);

    printf("(%lu, %lu)", clusters[0].centroid, clusters[0].point_count);
    printf("(%lu, %lu)", clusters[1].centroid, clusters[1].point_count);
}