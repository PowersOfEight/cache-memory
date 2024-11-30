typedef struct {
    long high_size;
    long low_size;
    long double delta;
} delta;

/**
 * Returns the highest delta
 * contained this run
 */
delta measure_cache_size();

