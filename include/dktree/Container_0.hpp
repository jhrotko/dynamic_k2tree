//
// Created by joana on 04/06/20.
//

#ifndef IMPLEMENTATION_CONTAINER_0_HPP
#define IMPLEMENTATION_CONTAINER_0_HPP

#include "edge.hpp"
#include "edge_hash_table.hpp"
#include "adjacency_list.hpp"

namespace dynamic_ktree {

    class Container_0 {
    public:
        Container_0() {}

        Container_0(size_t n_vertices) {
            const size_t max_edges = MAXSZ(n_vertices, 0);
            elements.resize(max_edges);
            n_elements = 0;

            edge_free.resize(max_edges);
            for (uint i = 0; i < max_edges; i++)
                edge_free[i] = i;

            adj_lst = adjacency_list(max_edges << 1);
            clean_free_lst();
        }

        void clean(size_t n_vertices) {
            edge_lst.clear();
            clean_free_lst();
            adj_lst.clear();
            elements.clear();
            n_elements = 0;
        }

        void clean_free_lst() {
            for (size_t i = 0; i < edge_free.size(); i++)
                edge_free[i] = i;
        }

        void insert(uint x, uint y) {
            if (edge_lst.find(x, y) == -1) {
                NodeDouble newNode(x, y);
                if (adj_lst[x] > -1 && adj_lst[x] < elements.size()) {
                    newNode.next = adj_lst[x];
                    elements[adj_lst[x]].prev = n_elements;
                    adj_lst.insert(x, n_elements);
                }
                edge_lst.insert(x, y, n_elements);
                elements[n_elements] = newNode;
                n_elements++;
            }
        }

        bool erase(uint x, uint y) {
            size_t nodeIndex = edge_lst.find(x, y);
            if (nodeIndex != -1) {
                if (elements[nodeIndex].next != -1)
                    elements[elements[nodeIndex].next].prev = elements[nodeIndex].prev;
                if (elements[nodeIndex].prev != -1)
                    elements[elements[nodeIndex].prev].next = elements[nodeIndex].next;
                else
                    adj_lst.insert(x, elements[nodeIndex].next);

                edge_free[elements.size()] = nodeIndex;
                return true;
            }
            return false;
        }

        void list_neighbours(uint x, vector<int> &neighbours) {
            int index = adj_lst[x];
            if (index != -1 && !elements.empty())
                for (; index != -1; index = elements[index].next)
                    neighbours.push_back(elements[index].y());
        }

        size_t size() const {
            return elements.size();
        }

        vector<NodeDouble>::const_iterator edge_begin() const { return elements.begin(); }
        vector<NodeDouble>::const_iterator edge_end() const { return elements.end(); }

        vector<Node>::const_iterator node_begin() const { return adj_lst.vertices.begin(); }
        vector<Node>::const_iterator node_end() const { return adj_lst.vertices.end(); }

        edge_hash_table edge_lst;
        vector <NodeDouble> elements;
        int n_elements;
        vector <uint> edge_free; //TODO: should go inside hash_table
        adjacency_list adj_lst;
    };
}
#endif //IMPLEMENTATION_CONTAINER_0_HPP
