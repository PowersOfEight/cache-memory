# Cache Memory Assignment
## CSCI 340 - Operating Systems
## Assignment 5
### By Dan Johnson & Brett Widholm

This assignment is designed to answer the following 4 questions through _experimentation_:

1. How big is a cache block for the current system?
2. How big is the cache in total?
3. What is the duration of an access to main memory?
4. What is the duration of an access to cache memory?

The language used is C, so we'll create all `.h` and `.c` files, along with `README.txt` (which will be created using `cat README.md > README.txt`), and the `Makefile`


### Methodology

We fill an array full of dummy integers based upon the buffer size and a set stride of 16. The buffer size indicates the size of each array in bits. We had a minimum buffer size of 1024 bits (128 bytes) and a maximum buffer size of $2^{26}$ bits (8388608 bytes), increasing the size of the buffer by a factor of 2 for each iteration. We perform accesses into the array in increments of our stride (accessing every 16th element in the array) until we reached the end, keeping a measure of these access times.

### Access Durations (3. & 4)
To obtain both the duration of accesses to cache memory and main memory, we congregated the data of access times into separate arrays based on what we believed to be cache misses and cache hits. A cache miss was deemed to occur when the access time into the array was at least 85% as long as the first access time. This is because we know that the first access time would always be a cache miss, so we could use it as a baseline for what a cache miss time could be. We found that 85% represented a fair floor for main memory access time because cache misses typically take anywhere from 10-100x as long as cache hits. Anything below this 85% threshold was deemed a cache miss. With both datasets aggregated into separate arrays, we sorted both arrays and took the median value in each array to obtain our answer.

### Block Size of the Cache
When we commenced our experiments, we started with a large array of bytes (`char *arr = (char*)malloc(ARR_SIZE)`), but found that this yielded inconsistent results.  Both mean and median data access times appeared to fluctuate wildly, and this made it difficult to achieve consistent results. However, when experimenting with different sizes, we found that we could use 32 or 64-bit integers to smooth out the times.  This was our first clue as to how to tackle the first question.  We found that, because the cache block (also known as the cache line) size was usually some even power of 2 between 32 and 128 bits wide, using an odd-sized _stride_ was forcing the computer to alter its fetching behavior.  

We found that increasing the step size for a given would result in a _spike_ in the mean access time when the size of the step was the equal to or larger than the cache block size.  To detect the spike, we looked for areas in the graph where the second derivative would be concave down with a local maximum.  We aggregated these into a list data structure and found the _mode_ spike to estimate the block size.

### Overall Cache Size

The cache is separated into 3 different levels on most systems.  A readout of `lscpu` showed us varying levels of cache (from L1 to L3, with L1 being split between data cache and instruction cache). Thus, to obtain the cache size, we decided to completely overwhelm the cache with an array that was simply too large to store within the sum of the caches.  We took the average of each access time for each buffer size and measured their differences. Each average access time for the current buffer size was subtracted from the average access time of previous buffer size and the magnitudes of change were stored (ΔT). We believe that the buffer size containing the largest ΔT is the size of the cache. This is because once cache is completely exhausted, there will be a large spike in access time that works towards our favor. Because we are taking the mean of the time, this brief spike in access time pollutes the data and sharply increases the average access time.