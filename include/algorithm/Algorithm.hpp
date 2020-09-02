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
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); ++edge_it) {
            edges_table.insert(edge_it.x(), edge_it.y(), index++);
        }
        // Given a node v, we can retrieve the nodes adjacent to v
        // in time proportional to the number of those nodes
        unordered_map<etype, vector<etype>> adj_node;
        for (auto node_it = g.node_begin(); node_it != g.node_end(); ++node_it) {
            etype node = *node_it;
            vector<etype> node_neighbour = g.list_neighbour(node);
            if (node_neighbour.size() == 0) continue;
            adj_node[node] = node_neighbour;
        }
        uint num_triangles = 0;
        for (auto edge_it = g.edge_begin(); edge_it != g.edge_end(); ++edge_it) {
            etype v1 = edge_it.x();
            etype v2 = edge_it.y();

            for (auto u = adj_node[v1].cbegin(); u != adj_node[v1].cend(); ++u) {
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

    static vector<float> pageRank(Graph &g, double convergence=0.00001, uint max_iterations=10000, float alpha=0.85) {
        uint num_rows = g.get_number_nodes();

        vector<uint> num_outgoing(num_rows);
        for(auto node_it = g.node_begin(); node_it != g.node_end(); node_it++) {
            num_outgoing[*node_it] = g.list_neighbour(*node_it).size();
        }

        uint num_iterations = 0;
        vector<float> pr(num_rows); pr[0] = 1;
        vector<float> old_pr;
        double diff = 1;

        while(diff > convergence && num_iterations < max_iterations) {
            double sum_pr = 0;
            double dangling_pr = 0;

            for (size_t k = 0; k < pr.size(); k++) {
                double cpr = pr[k];
                sum_pr += cpr;
                if (num_outgoing[k] == 0) {
                    dangling_pr += cpr;
                }
            }

            if (num_iterations == 0) {
                old_pr = pr;
            } else {
                /* Normalize so that we start with sum equal to one */
                for (size_t i = 0; i < pr.size(); i++) {
                    old_pr[i] = pr[i] / sum_pr;
                }
            }

            sum_pr = 1;
            /* An element of the A x I vector; all elements are identical */
            float one_Av = alpha * dangling_pr / num_rows;
            /* An element of the 1 x I vector; all elements are identical */
            float one_Iv = (1 - alpha) * sum_pr / num_rows;

            /* The difference to be checked for convergence */
            diff = 0;
            for (size_t i = 0; i < num_rows; i++) {
                /* The corresponding element of the H multiplication */
                float h = 0.0;
                uint64_t adj_node = 0;
                for (auto ci = g.neighbour_begin(i); ci != g.neighbour_end();) {
                    /* The current element of the H vector */
                    float h_v = (num_outgoing[adj_node])? 1.0f / num_outgoing[adj_node]: 0.0f;
                    h += h_v * old_pr[adj_node];

                    cout << "ci: " << *ci << endl;
                    if(adj_node < *ci) {
                        adj_node++;
                    } else if(adj_node == *ci) {
                        adj_node++;
                        ci++;
                    } else {
                        adj_node = *ci;
                        ci++;
                    }
                }
                h *= alpha;
                pr[i] = h + one_Av + one_Iv;
                diff += fabs(pr[i] - old_pr[i]);
            }
            num_iterations++;
        }

        return pr;
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
