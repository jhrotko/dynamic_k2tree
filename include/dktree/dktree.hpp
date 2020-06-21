#ifndef __D_K_TREE__
#define __D_K_TREE__
#define R 8

#include <array>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "utils.hpp"

#include <sdsl/k2_tree.hpp>
#include "Container_0.hpp"
#include "ktree_extended.hpp"
#include "dk_edge_iterator.hpp"
#include "dk_node_iterator.hpp"
#include "../graph/Graph.hpp"

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;


namespace dynamic_ktree {
    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type,
            typename l_rank = typename t_bv::rank_1_type>
    class dktree: public Graph<dk_edge_iterator<dktree<k, t_bv, t_rank, l_rank>, ktree_extended<k, t_bv, t_rank, l_rank>, edge_iterator<k, t_bv, t_rank>>> {
        using k_tree = ktree_extended<k, t_bv, t_rank, l_rank>;
        using k_tree_edge_it = edge_iterator<k, t_bv, t_rank>;
        using dktree_edge_it = dk_edge_iterator<dktree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_edge_it>;
        using dktree_node_it = dk_node_iterator<dktree<k, t_bv, t_rank, l_rank>>;
    public:
        dktree() {}
        dktree(uint n_vertices) : n_vertices(n_vertices) {
            C0 = Container_0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
        }

    private:
        uint max_r;
        uint n_vertices = 0;
        uint n_total_edges = 0;

        Container_0 C0;
        array<shared_ptr<k_tree>, R> k_collection;

        dktree_edge_it it_edge_begin, it_end;
        dktree_node_it node_it, node_it_end;
    public:
        virtual size_t get_number_edges() const {
            return n_total_edges;
        }

        virtual size_t get_number_nodes() const {
            return n_vertices;
        }

        virtual void add_edge(etype x, etype y)
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
                const tuple<idx_type, idx_type> e(C0.elements[j].x(), C0.elements[j].y());
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

        virtual bool contains(etype x, etype y)
        {
            if (C0.edge_lst.find(x, y) != -1)
                return true;

            // check in other containers
            for (size_t i = 0; i <= max_r; i++)
                if (k_collection[i] != nullptr && k_collection[i]->adj(x, y))
                    return true;
            return false;
        }

        virtual void del_edge(etype x, etype y)
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

        virtual vector<etype> list_neighbour(etype x)
        {
            vector<etype> neighbours;
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

        virtual dktree_edge_it &edge_begin()
        {
            it_edge_begin = dktree_edge_it(C0, &k_collection);
            return it_edge_begin;
        }

        virtual dktree_edge_it &edge_end()
        {
            it_end = it_edge_begin.end();
            return it_end;
        }

        dktree_node_it &node_begin()
        {
            node_it = dktree_node_it(this);
            return node_it;
        }

        dktree_node_it &node_end()
        {
            node_it_end = node_it.end();
            return node_it_end;
        }
    };
}

#endif