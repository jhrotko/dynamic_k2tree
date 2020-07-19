//
// Created by joana on 04/06/20.
//

#ifndef IMPLEMENTATION_CONTAINER_0_HPP
#define IMPLEMENTATION_CONTAINER_0_HPP

#include "EdgeHashTable.hpp"
#include "AdjacencyList.hpp"
#include "utils.hpp"

namespace dynamic_ktree {

    class Container_0 {
    private:
        etype n_elements;
        etype max_edges;
    public:
        Container_0() {}

        Container_0(size_t n_vertices) {
            max_edges = MAXSZ(n_vertices, 0);
            elements.resize(max_edges);
            n_elements = 0;

            edge_free.resize(max_edges);
            for (etype i = 0; i < max_edges; i++)
                edge_free[i] = i;

            adj_lst = AdjacencyList(max_edges << 1);
            clean_free_lst();
        }

        void clean(size_t n_vertices) {
            edge_lst.clear();
            clean_free_lst();
            adj_lst.clear();
            elements.clear();
            elements.resize(max_edges);
            n_elements = 0;
        }

        void clean_free_lst() {
            for (etype i = 0; i < max_edges; i++)
                edge_free[i] = i;
        }

        void insert(etype x, etype y) {
            if (!edge_lst.contains(x, y)) {
                NodeEdge newNode(x, y);
                if (!adj_lst[x].end() && adj_lst[x].next() < elements.size()) {
                    newNode.next(adj_lst[x].next());
                    elements[adj_lst[x].next()].prev(n_elements);
                    adj_lst.insert(x, n_elements);
                }
                edge_lst.insert(x, y, n_elements);
//                cout << n_elements << endl;
//                cout << elements.size() << endl;
//                assert(n_elements < elements.size());
                elements[n_elements] = newNode;
                n_elements++;
            }
        }

        bool erase(etype x, etype y) {
            unsigned int nodeIndex = edge_lst.find(x, y);
            if (nodeIndex != UINT_MAX) {
                if (elements[nodeIndex].has_next())
                    elements[elements[nodeIndex].next()].prev(elements[nodeIndex].prev());
                if (elements[nodeIndex].has_prev())
                    elements[elements[nodeIndex].prev()].next(elements[nodeIndex].next());
                else
                    adj_lst.insert(x, elements[nodeIndex].next());

                n_elements--;
                edge_free[n_elements] = nodeIndex;
                return true;
            }
            return false;
        }

        void list_neighbours(etype x, vector<etype> &neighbours) {
            Node node_adj = adj_lst[x];
            if (!node_adj.end() && !elements.empty())
                for (; !node_adj.end(); node_adj.next(elements[node_adj.next()].next()))
                    neighbours.push_back(elements[node_adj.next()].y());
        }

        size_t size() const {
            return n_elements;
        }

        etype max_size() const noexcept {
            return max_edges;
        }

        void resize(etype new_max_edges) {
            elements.resize(new_max_edges);
            edge_free.resize(new_max_edges);
            for(etype i = max_edges; i < new_max_edges; i++) {
                edge_free[i] = i;
            }
            max_edges = new_max_edges;
        }

        vector<NodeEdge>::const_iterator edge_begin() const { return elements.begin(); }
        vector<NodeEdge>::const_iterator edge_end() const { return elements.end(); }

        vector<Node>::const_iterator node_begin() const { return adj_lst.vertices.begin(); }
        vector<Node>::const_iterator node_end() const { return adj_lst.vertices.end(); }

        EdgeHashTable edge_lst;
        vector <NodeEdge> elements;
        vector <etype> edge_free;
        AdjacencyList adj_lst;
    };
}
#endif //IMPLEMENTATION_CONTAINER_0_HPP
