//
// Created by ray on 11.01.23.
//

#include "TreeNode.hpp"

span create_span(double start, double width) {
    return {start, start+width};
}

// point and region must agree on dimension!
bool is_within(const point& p, const region& r) {
    for (int i = 0; i<p.size(); i++) {
        auto value = p[i];
        span s = r[i];
        if (s.first > value or s.second < value) return false;
    }
    return true;
}



    TreeNode::TreeNode(region r): reg(std::move(r)){}

    TreeNode::TreeNode() {
        reg = {{0,0},{0,0}};
    }

    void TreeNode::create_subdivisions(bool force_division) {

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

    bool TreeNode::surrounds(const point& p) const {
        return is_within(p, reg);
    }

    bool TreeNode::empty() const{
        return this->data.empty();
    }

    bool TreeNode::has_subdivs() const {
        return !this->children.empty();
    }



    vector<point> TreeNode::approximate_k_nearest(const point& target, int k, vector<TreeNode> surrounding_nodes, vector<TreeNode> old_surrounding_nodes){

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

        /* 2D hardwired 'new_surrounding' rules
         *
         * ######
         * ######
         * ##--##
         * ##-s##
         * ######
         * ######
         *
         * # means surrounding
         * - means this
         * s means smaller_region (within this)
         *
         * The new_surrounding will consist of all direkt neighbors of 's'
         * */
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


