# An efficient KNN Approximation Algorithm
[K-Nearest-Neighbor (KNN)](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) is an algorithm used in maschine learning from classifying data.
This Project provides an efficient approximation algorithm for this problem.

#### What a KNN Algorithm needs to implement
* ***init***: creating some structure to store the data points and maybe something else
* ***find***: takes a target point and some k and gives the k data points closest to the target point 

In terms of complexity, the runtime of *init* and *find*, as well as the *storage* complexity are of interest.
Let n be the number of data points.

#### Naiv KNN Implementation
It may be enougth to store the data points in an array or list. To find the nearest neighbors, one could calculate the distance from the target point to all data points. Now the data points with the lowerst distances can be found. 

* *init*: O(n)
* *storage*: O(n)
* *find*: O(n)

*find* is guarantied to take at least O(n), becauso it is nesaccery to compute the distance to every data point. This has the downsite that calling *find* many times will be costly. The goal is therefore to reduce the cost of *find*.

#### Algorithm Idea
One could choose a tree datastructure for higher dimensions. It would divide space into smaller and smaller regions. With some luck, the knn are within a small region and most of the data points can be disregarded. That could bring *find* down to O(1), but there is a problem with edge cases: Assume all datapoints lie on a almost perfect circle around the target point. No tree structure could possibly calculate the nearest neighbors without regarding O(n) data points. 

Things change if we want to find a **approximation algorithm**. After all, a close enougth approximation may be good enought for most knn applications.
We want to ensure that every chosen "nearest neighbor" is at most **$\epsilon$-times** as far from the target as the closest not taken point.

Let us use a tree datastructure there every tree node has a list of data points that lie within the region it represents. For *find*, we recursivly go deeper in the tree. While doing this, we keep track of :
1. The **core**: the region we are currently searching in
2. The **inner surroundings**: all regions (of the same size) which directly surround the *core*
3. The **outer surroundings**: the *inner surroundings* of the last recursion level

The recursion terminates, then the number of data points in *core* plus *inner surroundings* is smaller or equal to k. All these points are taken to the solution and afterwards it is (randomly or in any manner) filled up with points from the *outer surroundings* until k points are selected. 

Having the relative region sizes of *inner / outer surrounding* in mind, one can calculate $\epsilon$ depending on the dimension of space. Most importantly, some $\epsilon$ is ensured to hold, because a not taken point has a minimum distance from the target point, because it has to lie outside of the *inner surroundings*.

Now let us have a look at the complexity. Since floating points have finite precision, we can assume the tree to have a fixed number of levels. Therefore:
* *init*: O(n)
* *storage*: O(n)

The *find* method will take therefore only constant steps to hit the base case. Taking the points within the *inner surroundings* and *core* and filling up takes k time. But if k is not depending on the number of data points and small anyway, we could simplify to:
* *find*: O(1)

#### Concrete Implementation
This implementation is working on 2-dimensional data and has $\epsilon$ $\le$ 6. For this implementation, the data points are assumed to be normalized to lie within [0,1]<sup>2</sup>. But this assumtion is in principle not required for the idea to work.

#### Conclusion
The concept can be extended to any dimension and the "surrounding idea" can be developed further to optain smaller $\epsilon$. this algorithm might be useful for real world applications, especially if many calles of the *find*-method are expected and an approximation is good enough.
