#ifndef __D_K_TREE__
#define __D_K_TREE__
#define R 8

#include <array>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>

#include "edge.hpp"
#include "edge_hash_table.hpp"
#include "adjacency_list.hpp"
#include <sdsl/k2_tree.hpp>
#include "ktree_extended.hpp"
#include "utils.hpp"
#include "dk_edge_iterator.hpp"
#include "../graph/Graph.hpp"

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;


namespace dynamic_ktree {
    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type,
            typename l_rank = typename t_bv::rank_1_type>
    class dktree: public Graph {

        using k_tree = ktree_extended<k, t_bv, t_rank, l_rank>;

    public:
        dktree() {}
        dktree(uint n_vertices) : n_vertices(n_vertices) {
            C0 = Container_0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
            it_begin = dk_edge_iterator<k, t_bv, t_rank, l_rank>(C0.elements, k_collection);
        }

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
                if (edge_lst.find(x, y) == -1)
                {
                    edge_node newNode(x, y);
                    if (adj_lst[x] != -1) {
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
                if (index != -1)
                    for (; index != -1; index = elements[index].next)
                        neighbours.push_back(elements[index].y);
            }

            vector<edge_node>::const_iterator begin() { return elements.begin(); }
            vector<edge_node>::const_iterator end() { return elements.end(); }

            edge_hash_table edge_lst;
            vector<edge_node> elements;
            int n_elements;
            vector<uint> edge_free; //should go inside hash_table
            adjacency_list adj_lst;
        };

    private:
        uint max_r;
        uint n_vertices;
        uint n_total_edges = 0;

        Container_0 C0;
        array<shared_ptr<k_tree>, R> k_collection;

        dk_edge_iterator<k, t_bv, t_rank, l_rank> it_begin, it_end;
    public:
        virtual size_t num_edges() const {
            return n_total_edges;
        }

        virtual void add_edge(uint x, uint y)
        {
            if (contains(x, y))
                return;
            size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
            if (C0.n_elements < max_size) {
                C0.insert(x, y);
                n_total_edges++;
                return;
            }

            size_t i = 0;
            for (; i < R; i++) {
                if (k_collection[i] != nullptr)
                    max_size += k_collection[i]->get_number_edges();
                if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > max_size + 1)
                    break;
            }

            if (i >= R)
                throw logic_error("Error: collection too big...");
            max_r = max(i, max_r);

            //Load edges in C0...
            vector<tuple<idx_type, idx_type>> free_edges;
            for (uint j = 0; j < C0.n_elements; j++) {
                const tuple<idx_type, idx_type> e(C0.elements[j].x, C0.elements[j].y);
                free_edges.push_back(e);
            }
            //Add new link...
            const tuple<idx_type, idx_type> e(x, y);
            free_edges.push_back(e);

            C0.clean(n_vertices);
            shared_ptr<k_tree> tmp = make_shared<k_tree>(free_edges, n_vertices);
            for (size_t j = 0; j <= i; j++) {
                if (k_collection[j] != nullptr) {
                    k_tree aux = tmp->unionOp(*k_collection[j], n_vertices);
                    tmp = make_shared<k_tree>(move(aux));
                }
                k_collection[j] = nullptr;
            }
            k_collection[i] = tmp;
            n_total_edges++;
        }

        virtual bool contains(uint x, uint y)
        {
            if (C0.edge_lst.find(x, y) != -1)
                return true;

            // check in other containers
            for (size_t i = 0; i <= max_r; i++)
                if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
                    return true;
            return false;
        }

        virtual void del_edge(uint32 x, uint32 y)
        {
            if (C0.erase(x, y)) n_total_edges--;
            else {
                uint n_total_marked = 0;
                for (size_t l = 0; l <= max_r; l++) {
                    if (k_collection[l] != nullptr && k_collection[l]->erase(x, y)) {
                        n_total_edges--;

                        uint k_marked_edges = k_collection[l]->get_marked_edges();
                        n_total_marked += k_marked_edges;

                        if (k_marked_edges == k_collection[l]->get_number_edges()) {
                            n_total_marked -= k_marked_edges;
                            delete &k_collection[l];
                            k_collection[l] = nullptr;
                        }
                    }
                }

                if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                    /* Rebuild data structure... */
                    max_r = 0;
                    for (size_t i = 0; i < R; i++) {
                        shared_ptr<k_tree> p(new k_tree(n_vertices));
                        k_collection[i] = p;
                    }
                }
            }
        }

        virtual vector<int> list_neighbour(uint x)
        {
            vector<int> neighbours;
            C0.list_neighbours(x, neighbours);

            for (size_t l = 0; l <= max_r; l++)
                if (k_collection[l] != nullptr) {
                    vector<idx_type> lst = k_collection[l]->neigh(x);
                    neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
                }
            return neighbours;
        }


        Container_0 first_container() const
        {
            return C0;
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &edge_begin()
        {
            it_begin = dk_edge_iterator<k, t_bv, t_rank, l_rank>(C0.elements, k_collection);
            return it_begin;
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &edge_end()
        {
            it_end = it_begin.end();
            return it_end;
        }
    };
}

#endif