#include <stdlib.h>

/**
 * size_t centroid: the center or _mean_
 * size_t* points : the array of points
 * size_t point_count: the number of points
 */
typedef struct{
    size_t centroid;
    size_t *points;
    size_t point_count;
} cluster;

/**
 * Calculates the k means of the data set
 */
void k_means(size_t *data, size_t data_size, size_t k, cluster* clusters);
size_t calculate_new_centroid(size_t *points, size_t point_count);
size_t closest_cluster(size_t value, cluster* clusters, size_t k);