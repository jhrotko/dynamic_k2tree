//
// Created by joana on 04/06/20.
//

#ifndef IMPLEMENTATION_CONTAINER_0_HPP
#define IMPLEMENTATION_CONTAINER_0_HPP

#include "EdgeHashTable.hpp"
#include "utils.hpp"

namespace dynamic_ktree {

    class Container_0 {
    private:
        etype n_elements;
        etype max_edges;
        size_t  n_vertices;
        const uint *n_edges;
    public:
        Container_0() {}

        Container_0(size_t n_vertices, const uint *n_edges) : n_vertices(n_vertices), n_edges(n_edges){
            max_edges = MAXSZ(n_vertices, 0);
            elements.resize(max_edges);
            elements_nodes.resize(max_edges);
            n_elements = 0;

            edge_free.resize(max_edges);
            for (etype i = 0; i < max_edges; i++)
                edge_free[i] = i;

//            adj_lst = AdjacencyList(max_edges << 1);
            adj_map.reserve(max_edges << 1);
            clean_free_lst();
        }

        void clean() {
            edge_lst.clear();
            clean_free_lst();
            adj_map.clear();

            elements.clear();
            elements.resize(max_edges);
            elements_nodes.clear();
            elements_nodes.resize(max_edges);

            n_elements = 0;
        }

        void clean_free_lst() {
            for (etype i = 0; i < max_edges; i++)
                edge_free[i] = i;
        }

        bool adj_contains(etype x) {
            return adj_map.find(x) != adj_map.end();
        }

        void insert(etype x, etype y) {
            if (!edge_lst.contains(x, y)) {
                if(n_elements >= elements.size()) {
                    resize(MAXSZ(max(n_vertices, *n_edges), 0));
                }
                etype i = edge_free[n_elements++];
                elements_nodes[i] = tuple<etype, etype>(x,y);
                if(!adj_contains(x))
                    adj_map[x] = i;
                else {
                    elements[i].next(adj_map[x]);
                    elements[adj_map[x]].prev(i);
                    adj_map[x] = i;
                }

                edge_lst.insert(get<0>(elements_nodes[i]), get<1>(elements_nodes[i]), i);
            }
        }

        bool erase(etype x, etype y) {
            unsigned int nodeIndex = edge_lst.find(x, y);
            if (nodeIndex != UINT_MAX) {
                edge_lst.erase(get<0>(elements_nodes[nodeIndex]),get<1>(elements_nodes[nodeIndex]));

                if (elements[nodeIndex].has_next())
                    elements[elements[nodeIndex].next()].prev(elements[nodeIndex].prev());
                if (elements[nodeIndex].has_prev())
                    elements[elements[nodeIndex].prev()].next(elements[nodeIndex].next());
                else
                    adj_map[get<0>(elements_nodes[nodeIndex])] = elements[nodeIndex].next();

                n_elements--;
                edge_free[n_elements] = nodeIndex;
                return true;
            }
            return false;
        }

        void list_neighbours(etype x, vector<etype> &neighbours) {
            if (adj_contains(x) && !elements.empty())
                for (auto neigh_it = adj_map.find(x); neigh_it != adj_map.end();  neigh_it = adj_map.find(elements[adj_map[neigh_it->first]].next())) {
                    neighbours.push_back(get<1>(elements_nodes[neigh_it->second]));
                }
        }

        size_t size() const {
            return n_elements;
        }

        etype max_size() const noexcept {
            return max_edges;
        }

        void resize(etype new_max_edges) {
            elements.resize(new_max_edges);
            elements_nodes.resize(new_max_edges);
            edge_free.resize(new_max_edges);
            for(etype i = max_edges; i < new_max_edges; i++) {
                edge_free[i] = i;
            }
            max_edges = new_max_edges;
        }

        vector<tuple<etype, etype>>::const_iterator edge_begin() const { return elements_nodes.begin(); }
        vector<tuple<etype, etype>>::const_iterator edge_end() const { return elements_nodes.end(); }

        unordered_map<etype, etype>::const_iterator node_begin() const { return adj_map.begin(); }
        unordered_map<etype, etype>::const_iterator node_end() const { return adj_map.end(); }

        EdgeHashTable edge_lst;
        unordered_map<etype, etype> adj_map; // x -> next

        vector <NodeEdge> elements; // next and prev
        vector<tuple<etype, etype>> elements_nodes; //x and y

        vector <etype> edge_free;
    };
}
#endif //IMPLEMENTATION_CONTAINER_0_HPP
