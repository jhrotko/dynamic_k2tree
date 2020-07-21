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
#include "DKtreeEdgeIterator.hpp"
#include "DKtreeNodeIterator.hpp"
#include "DKtreeNeighbourIterator.hpp"
#include "../graph/Graph.hpp"

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;


namespace dynamic_ktree {
    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type,
            typename l_rank = typename t_bv::rank_1_type>
    class DKtree: public Graph<
            DKtreeEdgeIterator<DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, edge_iterator<k2_tree<k, t_bv, t_rank, l_rank>>>,
            DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>,
            DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k2_tree<k, t_bv, t_rank, l_rank>, neighbour_iterator<k2_tree<k, t_bv, t_rank, l_rank>>>> {
        using k_tree = k2_tree<k, t_bv, t_rank, l_rank>;
        using k_tree_edge_it = edge_iterator<k_tree>;
        using k_tree_neighbour_it = neighbour_iterator<k_tree>;

        using dktree_edge_it = DKtreeEdgeIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_edge_it>;
        using dktree_node_it = DKtreeNodeIterator<DKtree<k, t_bv, t_rank, l_rank>>;
        using dktree_neighbour_it = DKtreeNeighbourIterator<DKtree<k, t_bv, t_rank, l_rank>, k_tree, k_tree_neighbour_it>;

    public:
        DKtree() {}
        DKtree(uint n_vertices) : n_vertices(n_vertices) {
            C0 = Container_0(n_vertices);
            max_r = 0;
            for (size_t i = 0; i < R; i++) {
                k_collection[i] = nullptr;
            }
        }

    private:
        uint max_r = 0;
        uint n_vertices = 0;
        uint n_total_edges = 0;

        Container_0 C0;
        array<shared_ptr<k_tree>, R> k_collection;

        dktree_edge_it it_edge_begin, it_end;
        dktree_node_it it_node_begin, it_node_end;
        dktree_neighbour_it it_neighbour_begin, it_neighbour_end;
    public:
        virtual size_t get_number_edges() const {
            return n_total_edges;
        }

        virtual size_t get_number_nodes() const {
            return n_vertices;
        }

        int get_max_r() const {
            return max_r;
        }

        virtual void add_edge(etype x, etype y)
        {
            if (n_vertices < 0){
                throw ("Unintialized dktree");
            }

            if (contains(x, y))
                return;
            size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
            if (C0.size() < max_size) {
                if(C0.max_size() < max_size)
                    C0.resize(max_size);
                C0.insert(x, y);
                n_total_edges++;
                return;
            }

            size_t i;
            for (i = 0; i < R; i++) {
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
            for (uint j = 0; j < C0.size(); j++) {
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
                    k_tree aux = tmp->unionOp(*k_collection[j]);
                    tmp = make_shared<k_tree>(move(aux));
                }
                k_collection[j] = nullptr;
            }
            k_collection[i] = tmp;
            n_total_edges++;
        }

        virtual bool contains(etype x, etype y)
        {
            if (C0.edge_lst.contains(x, y))
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
                        k_collection[i] = make_shared<k_tree>();
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

        array<shared_ptr<k_tree>, R> k_collections() const
        {
            return k_collection;
        }

        virtual dktree_edge_it &edge_begin()
        {
            it_edge_begin = dktree_edge_it(this);
            return it_edge_begin;
        }

        virtual dktree_edge_it &edge_end()
        {
            it_end = it_edge_begin.end();
            return it_end;
        }

        virtual dktree_node_it &node_begin()
        {
            it_node_begin = dktree_node_it(this);
            return it_node_begin;
        }

        virtual dktree_node_it &node_end()
        {
            it_node_end = it_node_begin.end();
            return it_node_end;
        }

        virtual dktree_neighbour_it &neighbour_begin(etype node)
        {
            it_neighbour_begin = dktree_neighbour_it(this, node);
            return it_neighbour_begin;
        }

        virtual dktree_neighbour_it &neighbour_end()
        {
            it_neighbour_end = it_neighbour_begin.end();
            return it_neighbour_end;
        }

        void load(std::istream &in) {
            sdsl::read_member(max_r, in);
            sdsl::read_member(n_vertices, in);
            sdsl::read_member(n_total_edges, in);
            C0 = Container_0(n_vertices);
            sdsl::read_member(C0, in);
//            sdsl::read_member(k_collection, in);
        }

        size_type serialize(std::ostream &out, structure_tree_node *v = nullptr,
                       std::string name = "") const {
            {
                structure_tree_node *child = structure_tree::add_child(
                        v, name, util::class_name(*this));
                size_type written_bytes = 0;

                written_bytes += write_member(max_r, out, child, "max_r");
                written_bytes += write_member(n_vertices, out, child, "n_vertices");
                written_bytes += write_member(n_total_edges, out, child, "n_total_edges");
                written_bytes += write_member(C0, out, child, "C0");
//                written_bytes += write_member(k_collection, out, child, "k_collection");
                structure_tree::add_size(child, written_bytes);
                return written_bytes;
            }
        }
    };
}

#endif