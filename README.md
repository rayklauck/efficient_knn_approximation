# An efficient KNN Approximation Algorithm
[K-Nearest-Neighbor (KNN)](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm) is an algorithm used in data science from classifying data.
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
1. The **Core**: the region we are currently searching in
2. The **inner Surroundings**: all regions (of the same size) which directly surround the *core*
3. The **outer Surroundings**: all regions (of same size) which surround the *inner surrounding*


#### Concrete Implementation
This implementation is working on 2-dimensional data and has $\epsilon$ $\le$ 6. 
