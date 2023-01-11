//
// Created by ray on 11.01.23.
//

#ifndef EFFICIENT_KNN_APPROXIMATION_KNN_HPP
#define EFFICIENT_KNN_APPROXIMATION_KNN_HPP

#include "TreeNode.hpp"

class  KNN {
    TreeNode top;

public:
    KNN(vector<point> data);

    // find k nearest neighbors of a point. Wrapping method of TreeNode
    vector<point> find(point p, int k = 1);
};

#endif //EFFICIENT_KNN_APPROXIMATION_KNN_HPP
