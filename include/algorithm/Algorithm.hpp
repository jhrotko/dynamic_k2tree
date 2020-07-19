#ifndef IMPLEMENTATION_ALGORITHM_HPP
#define IMPLEMENTATION_ALGORITHM_HPP

#include "../graph/Graph.hpp"
#include "../dktree/EdgeHashTable.hpp"
#include <deque>
#include <unordered_map>
#include <map>
#include <cmath>

using namespace std;

template<class Graph>
class Algorithm {
    using uint = unsigned int;
public:
    static vector<etype> bfs(Graph &g, uint root) {
        vector<etype> path;
        map<uint, bool> visited;
        deque<uint> queue;
        queue.push_back(root);
        visited[root] = true;

        while (!queue.empty()) {
            uint current_node = queue.front();
            queue.pop_front();
            path.push_back(current_node);

            vector<etype> neighbours = g.list_neighbour(current_node);
            for (int neigh: neighbours)
                if (!visited[neigh]) {
                    queue.push_back(neigh);
                    visited[neigh] = true;
                }
        }
        return path;
    }

    static vector<etype> dfs(Graph &g, uint root) {
        vector<etype> path;
        map<uint, bool> visited;
        deque<uint> stack;
        stack.push_front(root);
        visited[root] = true;

        while (!stack.empty()) {
            uint current_node = stack.front();
            stack.pop_front();
            path.push_back(current_node);

            vector<etype> neighbours = g.list_neighbour(current_node);
            for (int i = neighbours.size() - 1; i >= 0; i--) {
                int neigh = neighbours[i];
                if (!visited[neigh]) {
                    stack.push_front(neigh);
                    visited[neigh] = true;
                }
            }
        }
        return path;
    }

    static int count_triangles_dummy_hash(Graph &g) {
        // Create an index on edges, with the pair of nodes
        // at its ends as the key
        EdgeHashTable edges_table;
        uint index = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) {
            edges_table.insert(edge_it.x(), edge_it.y(), index++);
        }

        // Given a node v, we can retrieve the nodes adjacent to v
        // in time proportional to the number of those nodes
        unordered_map<etype, vector<etype>> adj_node;
        for (auto node_it = g.node_begin(); node_it != g.node_end(); node_it++) {
            etype node = *node_it;
            vector<etype> node_neighbour = g.list_neighbour(node);
            if (node_neighbour.size() == 0) continue;
            adj_node[node] = node_neighbour;
        }

        uint num_triangles = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) {
            etype v1 = edge_it.x();
            etype v2 = edge_it.y();

            for (auto u = adj_node[v1].cbegin(); u != adj_node[v1].cend(); u++) {
                if (edges_table.find(*u, v2) != -1)
                    num_triangles++;
            }
        }
        return num_triangles;
    }

    static unsigned int count_triangles_dummy(Graph &g) {
        unsigned int total_triangles = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) {
            etype v1 = edge_it.x();
            etype v2 = edge_it.y();
            for (auto neigh_v2_it = g.neighbour_begin(v2); neigh_v2_it != g.neighbour_end(); neigh_v2_it++) {
                etype v3 = *neigh_v2_it;
                if (v3 != v1) {
                    for (auto neigh_it = g.neighbour_begin(v3); neigh_it != g.neighbour_end(); neigh_it++) {
                        etype v1_maybe = *neigh_it;
                        if (v1 == v1_maybe) total_triangles++;
                    }
                }
            }
        }
        return total_triangles;
    }

    static unsigned int count_triangles(Graph &g) {
        // Create an index on edges, with the pair of nodes
        // at its ends as the key
        EdgeHashTable edges_table;
        uint index = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) {
            edges_table.insert(edge_it.x(), edge_it.y(), index++);
        }

        // Given a node v, we can retrieve the nodes adjacent to v
        // in time proportional to the number of those nodes
        unordered_map<etype, vector<etype>> adj_node;
        map<etype, etype> degree_node;
        for (auto node_it = g.node_begin(); node_it != g.node_end(); node_it++) {
            etype node = *node_it;
            if (g.list_neighbour(node).size() == 0) continue;
            adj_node[node] = g.list_neighbour(node);

            //Order nodes by degree
            degree_node[node] = adj_node[node].size();
        }

        uint num_triangles = 0;
        for (auto edge_it = edges_table.cbegin(); edge_it != edges_table.cend(); edge_it++) {
            etype v1 = edge_it->first.x();
            etype v2 = edge_it->first.y();
            if (is_heavy_hitter(g, degree_node[v1]) && is_heavy_hitter(g, degree_node[v2]))
                continue;

            else if (!is_heavy_hitter(g, degree_node[v1]) && is_heavy_hitter(g, degree_node[v2])
                     && minor_degree(v1, degree_node[v1], v2, degree_node[v2])) {
                for (auto u = adj_node[v1].cbegin(); u != adj_node[v1].cend(); u++) {
                    if (edges_table.contains(*u, v2) && minor_degree(v1, degree_node[v1], *u, degree_node[*u]))
                        num_triangles++;
                }
            }
        }

        return num_triangles;
    }

    static float clustering_coefficient(Graph &g) {
        float total_edges = g.get_number_edges();
        return count_triangles(g)/(total_edges*(total_edges-1));
    }

private:

    static bool is_heavy_hitter(Graph &g, uint node_degree) {
        return node_degree >= sqrt(g.get_number_edges());
    }

    static bool minor_degree(etype v1, uint degree_v1, etype v2, uint degree_v2) {
        if (degree_v1 < degree_v2) return true;
        return degree_v1 == degree_v2 && v1 < v2;
    }
};

#endif
