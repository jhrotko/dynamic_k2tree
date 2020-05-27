#ifndef K_TREE_EXTENDED_HPP
#define K_TREE_EXTENDED_HPP

#include <vector>
#include <map>

#include <sdsl/k2_tree.hpp>
#include <sdsl/bit_vectors.hpp>
#include "utils.hpp"

using namespace std;
using namespace sdsl;
using namespace k2_tree_ns;

template<uint8_t k = 2,
        typename t_bv = bit_vector,
        typename t_rank = typename t_bv::rank_1_type,
        typename l_rank = typename t_bv::rank_1_type>
class ktree_extended : public k2_tree<k, t_bv, t_rank, l_rank> {
    using  k_tree = k2_tree<k, t_bv, t_rank, l_rank>;

public:
    ktree_extended() : k_tree() {}

    ktree_extended(size_t n_vertices) : k_tree() {
        this->n_vertices = n_vertices;
    }

    ktree_extended(vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> edges,
                   const size_type size) : k_tree(edges, size) {
        this->n_vertices = size;
    }

    ktree_extended(k_tree tree, const size_t n_vertices) : k_tree(tree) {
        this->n_vertices = n_vertices;
    }

    ktree_extended unionOp(k_tree &k_tree, uint n_vertices) {
        k2_tree<2> res = k_tree.unionOp(*this);
        return ktree_extended(res, n_vertices);
    }

protected:
    uint n_vertices;
};

//template<class Item, uint8_t k = 2,
//        typename t_bv = bit_vector,
//        typename t_rank = typename t_bv::rank_1_type,
//        typename l_rank = typename t_bv::rank_1_type>
//class k_tree_item : public ktree_extended<k, t_bv, t_rank, l_rank> {
//    typedef ktree_extended<k, t_bv, t_rank, l_rank> k_tree;
//
//public:
//    k_tree_item(vector<tuple<Item, Item>> edges) : k_tree() {
//        assert(edges.size() > 0);
//
//        vector<tuple<idx_type, idx_type>> mapped_items;
//        idx_type current_idx = 0;
//        for (int i = 0; i < edges.size(); ++i) {
//            Item x = get<0>(edges[i]);
//            Item y = get<1>(edges[i]);
//
//            if (!items_map[x]) {
//                items_present.push_back(x);
//                items_map[x] = current_idx;
//                current_idx++;
//            }
//
//            if (!items_map[y]) {
//                items_present.push_back(y);
//                items_map[y] = current_idx;
//                current_idx++;
//            }
//            mapped_items.push_back(tuple<idx_type, idx_type>(items_map[x], items_map[y]));
//        }
//        sdsl::k2_tree<k, t_bv, t_rank, t_rank>::build_from_edges(mapped_items, items_present.size());
//        initialize(items_present.size());
//    }
//
//    k_tree_item(vector<tuple<Item, Item>> edges,
//                const size_type size) : k_tree(size) {
//        assert(size > 0);
//        assert(edges.size() > 0);
//
//        vector<tuple<idx_type, idx_type>> mapped_items;
//        idx_type current_idx = 0;
//        for (int i = 0; i < edges.size(); ++i) {
//            Item x = get<0>(edges[i]);
//            Item y = get<1>(edges[i]);
//
//            if (!items_map[x]) {
//                items_present.push_back(x);
//                items_map[x] = current_idx;
//                current_idx++;
//            }
//
//            if (!items_map[y]) {
//                items_present.push_back(y);
//                items_map[y] = current_idx;
//                current_idx++;
//            }
//            mapped_items.push_back(tuple<idx_type, idx_type>(items_map[x], items_map[y]));
//        }
//        sdsl::k2_tree<k, t_bv, t_rank, t_rank>::build_from_edges(mapped_items, size);
//    }
//
//    vector<Item> neigh(const Item &a) {
//        idx_type idx = items_map[a];
//        if (!idx) throw logic_error("The node does not exist.");
//
//        vector<idx_type> neighbours = k2_tree<k, t_bv, t_rank, l_rank>::neigh(idx);
//        vector<Item> res;
//        for (int i = 0; i < neighbours.size(); ++i) {
//            res.push_back(items_present[neighbours[i]]);
//        }
//        return res;
//    }
//
//    bool adj(const Item &a, const Item &b) {
//        idx_type idx_a = items_map[a];
//        idx_type idx_b = items_map[b];
//
//        if (!(idx_a && idx_b)) throw logic_error("The node does not exist.");
//
//        return k2_tree<k, t_bv, t_rank, l_rank>::adj(idx_a, idx_b);
//    }
//    //unionOp, adj, reverse_neigh, range, erase
//
//private:
//    map<Item, idx_type> items_map;
//    vector<Item> items_present;
//};

#endif //K_TREE_EXTENDED_HPP
