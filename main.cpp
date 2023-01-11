/* knn approximation algorithm. epsilon = 4*sqrt(2) < 6
 * e.g. none of the chosen points is further then epsilon times more distant that the nearest not taken point
 * this implementation is working in two dimensions
 *
 * Assumptions:
 * All Data-Points only occur once
 * for all points p1, p1: max(p1 - p2) / min(p1 - p2) is constant (assured by maschine numbers)
 * */

#include "TreeNode.hpp"
#include "KNN.hpp"


int main() {
    vector<point> data = {{1,1.0}, {0.3, 0.3}, {0.45, 0.3}, {0.1, 0.9}, {0.6, 0.3},{0.8,0.3}};
    KNN s = KNN(data);
    vector<point> res = s.find({0.9,0.9}, 3);

    for (auto r : res){
        cout << r[0] << " | " << r[1] << endl;
    }
    return 0;
}
