#define _POSIX_C_SOURCE 199309L
#include "cluster.h"
#include <limits.h>

#define MAX_ITER 1000000

void k_means(size_t *data, size_t data_size, size_t k, cluster* clusters) {
    
    // Initialize clusters
    for(size_t i = 0; i < k; i++) {
        clusters[i].centroid = data[i];
        clusters[i].points = malloc(data_size * sizeof(size_t));
        clusters[i].point_count = 0;
    }

    for(size_t iter = 0; iter < MAX_ITER; iter++) {
        
        for(size_t i=0; i < k; i++) {
            clusters[i].point_count = 0;
        }

        for(size_t i = 0; i < data_size; i++) {
            size_t cluster_index = closest_cluster(data[i], clusters, k);
            clusters[cluster_index].points[clusters[cluster_index].point_count++] = data[i];
        }

        int centroid_changed = 0;
        for(size_t i=0; i < k; i++) {
            size_t new_centroid = calculate_new_centroid(clusters[i].points, clusters[i].point_count);
            if (new_centroid != clusters[i].centroid) {
                clusters[i].centroid = new_centroid;
                centroid_changed = 1;
            }
        }
        if (centroid_changed == 0) break;
    }

}
size_t calculate_new_centroid(size_t *points, size_t point_count) {
    if(point_count == 0) return 0;
    size_t sum = 0;
    for(size_t i = 0; i < point_count; i++) {
        sum += points[i];
    }
    return sum / point_count;
}


size_t closest_cluster(size_t value, cluster* clusters, size_t k) {
    size_t min_distance = SSIZE_MAX;
    size_t closest = 0;

    for (size_t i = 0; i < k; i++) {
        size_t distance = abs((int)(value - clusters[i].centroid));
        if (distance < min_distance) {
            min_distance = distance;
            closest = i;
        }
    }
    return closest;
}