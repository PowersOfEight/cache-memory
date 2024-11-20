#include <stdlib.h>

typedef struct{
    size_t centroid;
    size_t *points;
    size_t point_count;
} cluster;


void k_means(size_t *data, size_t data_size, size_t k, cluster* clusters);
size_t calculate_new_centroid(size_t *points, size_t point_count);
size_t closest_cluster(size_t value, cluster* clusters, size_t k);