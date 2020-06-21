#ifndef IMPLEMENTATION_ALGORITHM_HPP
#define IMPLEMENTATION_ALGORITHM_HPP

#include "../graph/Graph.hpp"
#include "../dktree/EdgeHashTable.hpp"
#include <deque>
#include <map>

using namespace std;

template<class Graph>
class Algorithm {
    using uint = unsigned int;
public:
    static vector<int> bfs(Graph &g, uint root) {
        vector<int> path;
        map<uint, bool> visited;
        deque<uint> queue;
        queue.push_back(root);
        visited[root] = true;

        while (!queue.empty()) {
            uint current_node = queue.front(); queue.pop_front();
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

    static vector<int> dfs(Graph &g, uint root) {
        vector<int> path;
        map<uint, bool> visited;
        deque<uint> stack;
        stack.push_front(root);
        visited[root] = true;

        while (!stack.empty()) {
            uint current_node = stack.front(); stack.pop_front();
            path.push_back(current_node);

            vector<int> neighbours = g.list_neighbour(current_node);
            for (int i = neighbours.size()-1; i >= 0; i--) {
                int neigh = neighbours[i];
                if (!visited[neigh]) {
                    stack.push_front(neigh);
                    visited[neigh] = true;
                }
            }
        }
        return path;
    }

    static int count_triangles(Graph &g) {
        uint  n_nodes = g.get_number_edges();

        // Construct hash_edges;
        EdgeHashTable hash_edges;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) {
            cout << "hue" << endl;
        }

//        map<int, vector<int>> adj_node;
//        for (auto node = g.node_begin(); node != g.node_end(); node++) {
//            adj_node[node]
//        }
        return 0;
    }
};

#endif //IMPLEMENTATION_ALGORITHM_HPP
