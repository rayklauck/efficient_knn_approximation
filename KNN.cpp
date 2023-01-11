//
// Created by ray on 11.01.23.
//
/* knn approximation algorithm. epsilon = 4*sqrt(2) < 6
 * e.g. none of the chosen points is further then epsilon times more distant that the nearest not taken point
 * this implementation is working in two dimensions
 *
 * Assumptions:
 * All Data-Points only occur once
 * for all points p1, p1: max(p1 - p2) / min(p1 - p2) is constant (assured by maschine numbers)
 * */

#include "KNN.hpp"



    KNN::KNN(vector<point> data) {
        /* assuming all datapoints have coordinates 0 <= c <= 1 in every direction*/
        top.reg = {{0, 1}, {0, 1}};
        top.data = data;
        top.create_subdivisions();
    }

    // find k nearest neighbors of a point. Wrapping method of TreeNode
    vector<point> KNN::find(point p, int k) {

        // Filling neighborhood with placeholders to start the recursion
        vector<TreeNode> surrounding_nodes;
        for (int i = 0; i<8; i++) surrounding_nodes.emplace_back();

        return top.approximate_k_nearest(p,k,surrounding_nodes,surrounding_nodes);

    }
