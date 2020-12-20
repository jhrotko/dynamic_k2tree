#ifndef IMPLEMENTATION_ALGORITHM_HPP
#define IMPLEMENTATION_ALGORITHM_HPP

#include "../graph/Graph.hpp"
#include "../dktree/EdgeHashTable.hpp"
#include <deque>
#include <unordered_map>
#include <map>
#include <cmath>

using namespace std;

/*
 * Static class that implements the algorithms for classes that implement the Graph interface.
 */
template<class Graph>
class Algorithm {
    using uint = unsigned int;
public:
    //! Breadth First Searh
    /*!
     * Starts the search at root node
     * @param g graph to perform the search
     * @param root node where to start the search
     * @return a vector with the path
     */
    static vector<etype> bfs(Graph &g, etype root) {
        vector<etype> path;
        map<etype, bool> visited;
        deque<etype> queue;
        queue.push_back(root);
        visited[root] = true;

        while (!queue.empty()) {
            etype current_node = queue.front();
            queue.pop_front();
            path.push_back(current_node);

//            vector<etype> neighbours = g.list_neighbour(current_node);
//            for (int neigh: neighbours) {
            for (auto neigh_it = g.neighbour_begin(current_node); neigh_it != g.neighbour_end(); ++neigh_it) {
                etype neigh = *neigh_it;
                if (!visited[neigh]) {
                    queue.push_back(neigh);
                    visited[neigh] = true;
                }
            }
        }
        return path;
    }
    //! Depth First Searh
    /*!
     * Starts the search at root node
     * @param g graph to perform the search
     * @param root node where to start the search
     * @return a vector with the path
     */
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

    //! Counts the triangles with an hash table.
    /*!
     * This implementation uses a hash table to count the triangles.
     *  Takes O(|E|sqrt(|E|)) time and O(|V|+|E|) space.
     * @param g graph to perform the counting
     * @return the numbers of triangles in graph g
     */
    static int count_triangles_dummy_hash(Graph &g) {
        // Create an index on edges, with the pair of nodes
        // at its ends as the key
        EdgeHashTable edges_table;
        uint index = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); ++edge_it) { //O(log_k(n))
            edges_table.insert(edge_it.x(), edge_it.y(), index++);
        }
        // Given a node v, we can retrieve the nodes adjacent to v
        // in time proportional to the number of those nodes
        unordered_map<etype, vector<etype>> adj_node;
        for (auto node_it = g.node_begin(); node_it != g.node_end(); ++node_it) { //O(sqrt(m))
            etype node = *node_it;
            vector<etype> node_neighbour = g.list_neighbour(node); //O(sqrt(m))
            if (node_neighbour.size() == 0) continue;
            adj_node[node] = node_neighbour;
        }
        uint num_triangles = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); ++edge_it) { //O(log_k(n))
            etype v1 = edge_it.x();
            etype v2 = edge_it.y();
            if (v1 != v2) {
                for (auto u: adj_node[v2]) { //O(neighbours)
                    if (u != v1 && u != v2 && edges_table.contains(u, v1)) //O(log_k(n))
                        num_triangles++;
                }
            }
        }
        return num_triangles;
    }

    //! Counts the triangles using the neighbor iterator.
    /*!
     * This implementation iterates over the edges and neighbors to count the triangles.
     *  Takes O(|E|sqrt(|E|)log_k(|V|)log(|E|)) time and O(1) space.
     * @param g graph to perform the counting
     * @return the numbers of triangles in graph g
     */
    static unsigned int count_triangles_dummy(Graph &g, bool list=false) {
        unsigned int total_triangles = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); edge_it++) { //O(m)
            etype v1 = edge_it.x();
            etype v2 = edge_it.y();
            if (v1 != v2) {
                if (!list) {
                    for (auto neigh_v2_it = g.neighbour_begin(v2); neigh_v2_it != g.neighbour_end(); ++neigh_v2_it) { //O(sqrt(m))
                        etype v3 = *neigh_v2_it;
                        if (g.contains(v3, v1)) //O(log_k(n))
                            total_triangles++;
                    }
                } else {
                    auto neighbours = g.list_neighbour(v2);
                    for (auto n: neighbours) {
                        if (g.contains(n, v1))
                            total_triangles++;
                    }
                }
            }
        }
        return total_triangles; //O(m *(sqrt(m) + log_k(n)))
    }

    //! Counts the triangles haveing into consideration the heavy hitters.
    /*!
     * This implementation uses a hash table to count the triangles.
     *  Takes O(|E|sqrt(|E|)) time and O(|V|+|E|) space.
     * @param g graph to perform the counting
     * @return the numbers of triangles in graph g
     */
    static unsigned int count_triangles(Graph &g) {
        // Create an index on edges, with the pair of nodes
        // at its ends as the key
        EdgeHashTable edges_table;
        uint index = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); ++edge_it) {
            edges_table.insert(edge_it.x(), edge_it.y(), index++);
        }

        // Given a node v, we can retrieve the nodes adjacent to v
        // in time proportional to the number of those nodes
        unordered_map<etype, vector<etype>> adj_node;
        map<etype, etype> degree_node;
        for (auto node_it = g.node_begin(); node_it != g.node_end(); ++node_it) {
            etype node = *node_it;
            if (g.list_neighbour(node).size() == 0) continue;
            adj_node[node] = g.list_neighbour(node);

            //Order nodes by degree
            degree_node[node] = adj_node[node].size();
        }

        uint num_triangles = 0;
        for (auto edge_it = edges_table.cbegin(); edge_it != edges_table.cend(); ++edge_it) {
            etype v1 = edge_it->first.x();
            etype v2 = edge_it->first.y();
            if (is_heavy_hitter(g, degree_node[v1]) && is_heavy_hitter(g, degree_node[v2]))
                continue;

            else if (!is_heavy_hitter(g, degree_node[v1]) && is_heavy_hitter(g, degree_node[v2])
                     && minor_degree(v1, degree_node[v1], v2, degree_node[v2])) {
                for (auto u = adj_node[v1].cbegin(); u != adj_node[v1].cend(); ++u) {
                    if (edges_table.contains(*u, v2) && minor_degree(v1, degree_node[v1], *u, degree_node[*u]))
                        num_triangles++;
                }
            }
        }

        return num_triangles;
    }

    //! Calculates the clustering Coefficient of a graph.
    /*!
     * @param g graph to perform the counting
     * @return the clustering coefficient for graph g
     */
    static float clustering_coefficient(Graph &g) {
        float total_edges = g.get_number_edges();
        return count_triangles(g) / (total_edges * (total_edges - 1));
    }

    //! Calculates the pageRank for a graph.
    /*!
     * @param g graph to perform the pagerank
     * @return the ranks for each node of g
     */
    static unordered_map<uint, double>
    pageRank(Graph &g, double convergence = 1.0e-6f, uint max_iterations = 100, double alpha = 0.85f) {
        double N = (double) g.get_number_nodes();
        double total_nodes = 0.0f;

        vector<vector<etype>> outgoing_link(N);
        for (size_t i = 0; i < N; i++) {
            outgoing_link[i] = g.list_neighbour(i);
            if (!outgoing_link[i].empty())
                total_nodes++;
        }

        vector<uint> dangling_nodes;
        for (size_t i = 0; i < N; ++i)
            if (outgoing_link[i].empty())
                dangling_nodes.push_back(i);

        vector<double> pr(N);
        pr.assign(N, 1.0f / total_nodes);
        vector<double> old_pr(N);

        uint64_t step = 0;
        double dangling_sum; // sum of current pagerank vector elements for dangling
        while (step < max_iterations) {
            old_pr = pr;
            pr.assign(N, 0);

            dangling_sum = 0.0f;
            for (auto k: dangling_nodes) {
                dangling_sum += old_pr[k];
            }
            dangling_sum *= alpha;

            for (size_t i = 0; i < N; i++) {
                /* The corresponding element of the H multiplication */
                for (uint64_t neigh: outgoing_link[i]) {

                    /* The current element of the H vector */
                    double k = !outgoing_link[i].empty() ? 1.0f / outgoing_link[i].size() : 0.0f;
                    pr[neigh] += alpha * old_pr[i] * k;
                }

                pr[i] += dangling_sum * (1.0f / N) + (1.0f - alpha) * (1.0f / N);
                //Check Convergence
                double err = 0.0f;
                for (size_t k = 0; k < N; k++) {
                    err += abs(pr[k] - old_pr[k]);
                }
                if (err < N * convergence) {
                    unordered_map<uint, double> res;
                    for (size_t j = 0; j < N; j++) {
                        res[j] = pr[j];
                    }
                    return res;
                }
            }
            step++;
        }

        unordered_map<uint, double> res;
        for (size_t i = 0; i < N; i++) {
            res[i] = pr[i];
        }
        return res;
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
