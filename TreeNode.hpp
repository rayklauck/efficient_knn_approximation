//
// Created by ray on 11.01.23.
//

#ifndef EFFICIENT_KNN_APPROXIMATION_TREENODE_HPP
#define EFFICIENT_KNN_APPROXIMATION_TREENODE_HPP

#include <utility>
#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;
typedef vector<double> point;
typedef pair<double, double> span;
typedef vector<span> region;

span create_span(double start, double width);

bool is_within(const point& p, const region& r);


struct TreeNode {
    vector<TreeNode> children; // will be of length 4  (2**dimensions)
    vector<point> data;
    region reg;                 // in 2-dimentional space

    TreeNode(region r);

    TreeNode();

    void create_subdivisions(bool force_division = false);

    bool surrounds(const point& p) const;

    bool empty() const;

    bool has_subdivs() const;

    vector<point> approximate_k_nearest(const point& target, int k, vector<TreeNode> surrounding_nodes, vector<TreeNode> old_surrounding_nodes);
};


#endif //EFFICIENT_KNN_APPROXIMATION_TREENODE_HPP
