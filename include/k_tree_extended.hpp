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
class k_tree_extended : public k2_tree<k, t_bv, t_rank, l_rank> {
    typedef k2_tree<k, t_bv, t_rank, l_rank> k_tree;

public:
    k_tree_extended() : k_tree() {}

    k_tree_extended(size_t n_vertices) : k_tree() {
        initialize(n_vertices);
    }

    k_tree_extended(vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> edges,
                    const size_type size) : k_tree(edges, size) {
        uint max_v = 0;


        for (size_t i = 0; i < edges.size(); i++) {
            max_v = max(max_v, get<0>(edges[i]));
            max_v = max(max_v, get<1>(edges[i]));
        }
        if (max_v > size)
            throw logic_error("Incorrect size. It must be higher than the highest node");

        initialize(size);
    }

    k_tree_extended(k_tree tree, const size_t n_vertices) : k_tree(tree) {
        initialize(n_vertices);
    }


    void edge_iterator(function<int(uint, uint)> callback) {
        vector<ulong> pointerL;
        pointerL.resize(max_level);
        pointerL.push_back(0);
        pointerL.push_back(k * k);

        for (size_t i = 2; i <= max_level; i++) {
            ulong a = (this->k_l_rank(pointerL[i - 1] - 1) + 1) * k * k;
            pointerL.push_back(a);
        }

        recursive_edge_iterator(pointerL, 0, 0, -1, -1, callback);
    }

    k_tree_extended unionOp(k_tree &k_tree, uint n_vertices) {
        k2_tree<2> res = k_tree.unionOp(*this);
        return k_tree_extended(res, n_vertices);
    }

    void set_level(vector<int> div_level_table) {
        this->div_level_table = div_level_table;
    }

protected:
    void initialize(uint size) {
        n_vertices = size;
        uint aux = log(n_vertices) / log(k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);

        for (size_t i = 0; i <= max_level; i++)
            div_level_table.push_back(pow(k, max_level - i));
    }

    void
    recursive_edge_iterator(vector<ulong> pointerL, uint dp, uint dq, uint x, int l, function<int(uint, uint)> proc) {
        uint y;
        if (l == max_level)
            if (this->k_l[x])
                proc(dp, dq);

        if ((l == max_level - 1) && (this->k_l[x])) {
            y = pointerL[l + 1];
            pointerL[l + 1] += k * k;
            for (size_t i = 0; i < k; i++)
                for (size_t j = 0; j < k; j++)
                    recursive_edge_iterator(pointerL, dp + i, dq + j, y + k * i + j, l + 1, proc);
        }
        if ((x == -1) || ((l < max_level - 1) && (this->k_l[x]))) {
            y = pointerL[l + 1];
            pointerL[l + 1] += k * k;

            uint div_level = div_level_table[l + 1];
            for (size_t i = 0; i < k; i++)
                for (size_t j = 0; j < k; j++)
                    recursive_edge_iterator(pointerL, dp + div_level * i, dq + div_level * j, y + k * i + j, l + 1,
                                            proc);
        }
    }

    vector<int> div_level_table;
    uint max_level;
    uint n_vertices;
};

//template<class Item, uint8_t k = 2,
//        typename t_bv = bit_vector,
//        typename t_rank = typename t_bv::rank_1_type,
//        typename l_rank = typename t_bv::rank_1_type>
//class k_tree_item : public k_tree_extended<k, t_bv, t_rank, l_rank> {
//    typedef k_tree_extended<k, t_bv, t_rank, l_rank> k_tree;
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
