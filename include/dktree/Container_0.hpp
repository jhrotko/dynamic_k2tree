#ifndef IMPLEMENTATION_CONTAINER_0_HPP
#define IMPLEMENTATION_CONTAINER_0_HPP

#include "EdgeHashTable.hpp"
#include "utils.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/access.hpp>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>

namespace dynamic_ktree {

    class Container_0 {
    private:
        uint64_t n_elements, max_edges;
        uint64_t n_vertices;

    public:
        Container_0() {}

        Container_0(size_t n_vertices) : n_vertices(n_vertices) {
            max_edges = MAXSZ(n_vertices, 0);

            edge_lst.reserve(max_edges);
            elements.resize(max_edges);
            elements_nodes.resize(max_edges);
            edge_free.resize(max_edges);
            for (etype i = 0; i < max_edges; i++)
                edge_free[i] = i;
            adj_map.reserve(max_edges << 1);
            n_elements = 0;
        }

        void clean() {
            n_elements = 0;
            marked = 0;

            edge_lst = EdgeHashTable();
            edge_lst.reserve(max_edges);
            adj_map = unordered_map<etype, etype>(max_edges*2);

            elements = vector<NodeEdge>(max_edges);
            elements_nodes = vector<Edge>(max_edges);
            edge_free = vector<int64_t>(max_edges);

            for (etype i = 0; i < max_edges; i++) {
                edge_free[i] = i;
            }
        }

        bool adj_contains(etype x) {
            return adj_map.find(x) != adj_map.end();
        }

        bool contains(etype x, etype y) {
            return edge_lst.contains(x, y);
        }

        void insert(etype x, etype y, uint64_t n_edges) {
            if (!edge_lst.contains(x, y)) {
                if (n_elements >= elements.size()) {
                    resize(MAXSZ(max(n_vertices, n_edges), 0));
                }
                etype i = edge_free[n_elements];
                n_elements++;
                elements_nodes[i] = Edge(x, y);
                edge_lst.insert(x, y, i);

                if (!adj_contains(x))
                    adj_map[x] = i;
                else {
                    elements[i].next(adj_map[x]);
                    elements[adj_map[x]].prev(i);
                    adj_map[x] = i;
                }
            }
        }

        bool erase(etype x, etype y) {
            int64_t nodeIndex = edge_lst.find(x, y);
            if (nodeIndex != -1) {
                edge_lst.erase(x, y);

                if(!elements[nodeIndex].has_next() && !elements[nodeIndex].has_prev()) { // empty
                    adj_map.erase(x);
                }
                if (!elements[nodeIndex].has_next() && elements[nodeIndex].has_prev()) { //last
                    elements[elements[nodeIndex].prev()].remove_next();
                }
                else if (elements[nodeIndex].has_next() && !elements[nodeIndex].has_prev()) { //first
                    elements[elements[nodeIndex].next()].remove_prev();
                    adj_map[x] = elements[nodeIndex].next();

                } else if(elements[nodeIndex].has_next() && elements[nodeIndex].has_prev()){
                    elements[elements[nodeIndex].next()].prev(elements[nodeIndex].prev());
                    elements[elements[nodeIndex].prev()].next(elements[nodeIndex].next());
                }
                edge_free[nodeIndex] = -1;
                marked++;
                return true;
            }
            return false;
        }

        void list_neighbours(etype x, vector<etype> &neighbours) {
            if (adj_contains(x) && !elements.empty()) {
                bool done = false;
                size_t k = adj_map[x];
                while(!done) {
                    neighbours.push_back(elements_nodes[k].y());

                    if(elements[k].has_next()) {
                        k = elements[k].next();
                    } else {
                        done = true;
                    }
                }
            }
        }

        etype size() const {
            return n_elements == 0? 0: n_elements-marked;
        }

        etype size_non_marked() {
            return n_elements;
        }

        etype max_size() const noexcept {
            return max_edges;
        }

        void resize(uint64_t new_max_edges) {
            if (new_max_edges > max_edges) {
                elements.resize(new_max_edges);
                elements_nodes.resize(new_max_edges);
                edge_free.resize(new_max_edges);
                for (etype i = n_elements; i < new_max_edges; i++) {
                    edge_free[i] = i;
                }
                max_edges = new_max_edges;

            }
        }

        bool operator==(const Container_0 &rhs) const {
            bool eval = true;
            eval &= n_elements == rhs.n_elements;
            eval &= max_edges == rhs.max_edges;
            eval &= n_vertices == rhs.n_vertices;

            eval &= edge_lst == rhs.edge_lst;
            eval &= adj_map == rhs.adj_map;

            eval &= elements == rhs.elements;
            eval &= elements_nodes.size() == rhs.elements_nodes.size();
            for (unsigned int i = 0; i < n_elements; i++)
                eval &= elements_nodes[i] == rhs.elements_nodes[i];
            eval &= edge_free == rhs.edge_free;

            return eval;
        }

        bool operator!=(const Container_0 &rhs) const {
            return !(*this == rhs);
        }

        vector<Edge>::const_iterator edge_begin() const { return elements_nodes.begin(); }

        vector<Edge>::const_iterator edge_end() const { return elements_nodes.end(); }

        unordered_map<etype, etype>::const_iterator node_begin() const { return adj_map.begin(); }

        unordered_map<etype, etype>::const_iterator node_end() const { return adj_map.end(); }

        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int) {
            ar & n_elements;
            ar & n_vertices;
            ar & max_edges;

            ar & edge_free;
            ar & elements_nodes;
            ar & elements;
            ar & adj_map;
            ar & edge_lst;
        }

        template<class Archive>
        void load(Archive &ar, const unsigned int) {
            ar >> n_elements;
            ar >> n_vertices;
            ar >> max_edges;

            ar >> edge_free;
            ar >> elements_nodes;
            ar >> elements;
            ar >> adj_map;
            ar >> edge_lst;
        }


        EdgeHashTable edge_lst;
        unordered_map<etype, etype> adj_map; // x -> next

        vector<NodeEdge> elements; // next and prev
        vector<Edge> elements_nodes; //x and y

        vector<int64_t> edge_free;
        etype marked = 0;
    };
}
#endif //IMPLEMENTATION_CONTAINER_0_HPP
