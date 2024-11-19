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


### Thoughts on answering 3 & 4

If we allocate some memory, we can populate a list of data-points with the time it takes to do some basic read to it.  Speaking abstractly, we're going to create $n \in \mathbb{N}$ separate data points that will be _access times_.  Let $X$ be the set of all data-points generated this way.  These data points will literally be the time it takes (in nanoseconds) to access this chunk of memory.  We should have mostly cache hits and a few cache misses.  If we put these on a number line, we could try every subset of $k$ centroids and find which one produces the _minimum variance_.  This is essentially the $k$-means algorithm. 

What we're trying to do is _cluster_ the data points into two groups on a number line, so this is straightforward, and we can check every data point for $k$ clusters using a brute force method in $\Theta(kn^{k+1})$ time complexity.

$\textbf{\text{Proof: }}$ Let $n$ be the number of data points, and $k$ the number of clusters we're seeking.  Let $m$ be the number of unique cardinality $k$ subsets of centroids we need to produce.  We have that
$$
\begin{align*}
    m &= {n \choose k} \\
    &= \frac{n!}{(n - k)!}\\
    &= \frac{n \cdot (n-1) \cdot \cdots \cdot (n - (k - 1)) \cdot \cancel{(n - k)!}}{\cancel{(n-k)!}}\\
    &= \prod_{i = 0}^{k - 1} \left(n - i\right) \in O(n^{k})
\end{align*}
$$

Let $x_i \in X$, and let centroid $c_j$ for $j \in [k]$, then to calculate the minimum total variance, 
$$
    \text{var}_{\text{min}} = \text{min}_{j\in[k]}(x_i - c_j)^2
$$

This takes $k$ passes to try, then we take the total for this iteration and get

$$
    \sum_{i=1}^n \text{min}_{j \in [k]}(x_i - c_j)^2
$$
This takes us $n \cdot k$ steps, and so we have that the total time complexity is for constants $l_1, l_2$ is 
$$
    l_1 \cdot  n^k \cdot l_2 \cdot k \cdot n \in O(kn^{k+1})
$$$ \square$


[Click here for some explanation of the strategy](https://lemire.me/blog/2023/12/12/measuring-the-size-of-the-cache-line-empirically/)