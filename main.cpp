//
// Created by ray on 09.01.23.
//
#include <utility>
#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;
typedef vector<double> point;
typedef pair<double, double> span;
typedef vector<span> region;

/* 4*sqrt(2) - approximation algorithm
 * e.g. none of the chosen points is further then 4*sqrt(2) times more distant that the nearest not taken point
 *
 * Assumptions: All Data-Points only occur once
 * max(delta points) / min(delta points)   is constant (assured by maschine numbers)
 * lets start with two dimensions
 * */

span create_span(double start, double width) {
    return {start, start+width};
}

// point and region must agree on the dimension!
bool is_within(const point& p, const region& r) {
    for (int i = 0; i<p.size(); i++) {
        auto value = p[i];
        span s = r[i];
        if (s.first > value or s.second < value) return false;
    }
    return true;
}


struct TreeNode {
    vector<TreeNode> children; // will be of length 4  (2**dimensions)
    vector<point> data;
    region reg; // in high-dimentional space

    TreeNode(region r): reg(std::move(r)){}

    TreeNode() {
        reg = {{0,0},{0,0}};
    }

    void create_subdivisions(bool force_division = false) {

        // do not consider empty regions
        if (empty() and not force_division) {
            return;
        }

        // do not divide further if only one datapoint is contained
        if (data.size() == 1 and not force_division) {
            return;
        }

        // creating subregions
        // assuming the regions to be squares
        double dx2 = (reg[0].second - reg[0].first) / 2.0;
        children.emplace_back(region{create_span(reg[0].first, dx2), create_span(reg[1].first, dx2)} );
        children.emplace_back(region{create_span(reg[0].first + dx2, dx2), create_span(reg[1].first, dx2)} );
        children.emplace_back(region{create_span(reg[0].first, dx2), create_span(reg[1].first + dx2, dx2)} );
        children.emplace_back(region{create_span(reg[0].first + dx2, dx2), create_span(reg[1].first + dx2, dx2)} );

        // splitting up data points
        for (auto d : data) {
            for (auto& t : children) {
                if (t.surrounds(d)) {
                    t.data.push_back(d);
                    break; // so that a point can only be in one set
                }
            }
        }

        // recursive calls
        for (auto& t : children) {
            t.create_subdivisions();
        }
    }

    bool surrounds(const point& p) const {
        return is_within(p, reg);
    }

    //empty Nodes are called 'placeholder'
    bool empty() const{
        return this->data.empty();
    }

    bool has_subdivs() const {
        return !this->children.empty();
    }



    vector<point> approximate_k_nearest(const point& target, int k, vector<TreeNode> surrounding_nodes, vector<TreeNode> old_surrounding_nodes){

        // Base Case where <= k points are in scope
        size_t data_points_count_in_scope = data.size();
        for (auto& t : surrounding_nodes) {
            data_points_count_in_scope += t.data.size();
        }
        if (data_points_count_in_scope <= k) {
            vector<point> chosen = data;    // choose local points first

            // then all from local environment
            for (auto& t : surrounding_nodes) {
                chosen.insert(chosen.end(), t.data.begin(), t.data.end());
            }

            if (chosen.size() == k) return chosen;

            // fill with some points from larger environment (take only those who are not already in local environment)
            int still_required = k-chosen.size();
            for (int i = 0; i<old_surrounding_nodes.size(); i++){
                for (int j = 0; j<old_surrounding_nodes[i].data.size(); j++) {
                    point p = old_surrounding_nodes[i].data[j];
                    if (find(chosen.begin(), chosen.end(), p) == chosen.end()) { // check that point was not already taken from local environment (inefficient implementation)
                        chosen.push_back(p);
                        still_required--;
                        if (still_required<=0) return chosen;  // should be guranteed to occur
                    }

                }
            }
            return chosen;
            assert(false); // should never come here
        }
        // End Base Case


        // filling empty environment with more granularity
        for (auto& t : surrounding_nodes) {
            if (!t.has_subdivs()){
                t.create_subdivisions(true); // force finer granularity for one level
            }
        }
        if (!has_subdivs()) create_subdivisions(true);


        // computing next finer regions
        TreeNode smaller_region;
        vector<TreeNode> new_surrounding;

        // bad 2D Boilerplate
        if (children[0].surrounds(target)) {
            smaller_region = children[0];
            new_surrounding = {surrounding_nodes[7].children[3], children[2],children[3],children[1],surrounding_nodes[5].children[3],surrounding_nodes[5].children[2],
                                            surrounding_nodes[6].children[3],surrounding_nodes[7].children[1]};
        } else if (children[1].surrounds(target)) {
            smaller_region = children[1];
            new_surrounding = {children[2], children[3],surrounding_nodes[3].children[2],surrounding_nodes[3].children[0],surrounding_nodes[4].children[2],
                                                surrounding_nodes[5].children[3],surrounding_nodes[5].children[2],children[0]};
        }
        else if (children[2].surrounds(target)) {
            smaller_region = children[2];
            new_surrounding = {surrounding_nodes[0].children[1],surrounding_nodes[1].children[0], surrounding_nodes[1].children[1],
                               children[3], children[1],children[0],surrounding_nodes[7].children[1],surrounding_nodes[7].children[3]};
        }
        else if (children[3].surrounds(target)) {
            smaller_region = children[3];
            new_surrounding = {surrounding_nodes[1].children[0],surrounding_nodes[1].children[1], surrounding_nodes[2].children[0],
                               surrounding_nodes[3].children[2], surrounding_nodes[3].children[0],children[1], children[0],children[2]};
        } else {
            throw "must be in one of these partitions!";
        }


        // recursive call
        return smaller_region.approximate_k_nearest(target,k,new_surrounding, surrounding_nodes);
    }
};



struct KNN {
    TreeNode top;

    KNN(int dimension, vector<point> data) {
        /* assuming all datapoints have coordinates 0 <= c <= 1 in every direction*/
        top.reg = {{0, 1}, {0, 1}};
        top.data = data;
        top.create_subdivisions();
    }

    // find k nearest neighbors of a point. Wrapping method of TreeNode
    vector<point> operator()(point p, int k = 1) {

        // Filling neighborhood with placeholders
        vector<TreeNode> surrounding_nodes;
        for (int i = 0; i<8; i++) surrounding_nodes.emplace_back();

        return top.approximate_k_nearest(p,k,surrounding_nodes,surrounding_nodes);

    }
};

int main() {
    vector<point> data = {{1,1.0}, {0.3, 0.3}, {0.45, 0.3}, {0.1, 0.9}, {0.6, 0.3},{0.8,0.3}};
    KNN s = {2,data};
    vector<point> res = s({0.9,0.9}, 3);

    /*
    TreeNode t = region {{0,1},{0,1}};
    t.data  = data;
    t.create_subdivisions();
    t.approximate_k_nearest({0.26,0.26},1,{TreeNode()},TreeNode(),{TreeNode()});
    vector<point> res = t.approximate_k_nearest({0.26,0.26},1,{},t,{});
     */
    for (auto r : res){
        cout << r[0] << " | " << r[1] << endl;
    }
    return 0;
}
