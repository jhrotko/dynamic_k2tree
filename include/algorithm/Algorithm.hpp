#ifndef IMPLEMENTATION_ALGORITHM_HPP
#define IMPLEMENTATION_ALGORITHM_HPP

#include "../graph/Graph.hpp"
#include <deque>
#include <iostream>
#include <map>

using namespace std;

class Algorithm {
    using uint = unsigned int;
public:
    static vector<int> bfs(Graph &g, uint root) {
        vector<int> path;
        map<uint, bool> visited;
        deque<uint> queue;
        queue.push_back(root);

        while (!queue.empty()) {
            uint current_node = queue.front();
            queue.pop_front();
            path.push_back(current_node);

            vector<int> neighbours = g.list_neighbour(current_node);
            for (int neigh: neighbours)
                if (!visited[neigh]) {
                    queue.push_back(neigh);
                    visited[neigh] = true;
                }
        }
        return path;
    }
};

#endif //IMPLEMENTATION_ALGORITHM_HPP
