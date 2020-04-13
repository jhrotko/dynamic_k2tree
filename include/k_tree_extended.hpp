#ifndef K_TREE_EXTENDED_HPP
#define K_TREE_EXTENDED_HPP

#include <vector>

#include <sdsl/k2_tree.hpp>
#include "utils.hpp"

using namespace std;
using namespace sdsl;
using namespace k2_tree_ns;

class k2_tree_extended : public k2_tree<2>
{
public:
    k2_tree_extended(uint n_vertices) : k2_tree()
    {
        initialization(n_vertices);
    }

    k2_tree_extended(vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> edges,
                     const size_type size) : k2_tree(edges, size)
    {
        uint max_v = 0;

        for (size_t i = 0; i < edges.size(); i++)
        {
            max_v = max(max_v, get<0>(edges[i]));
            max_v = max(max_v, get<1>(edges[i]));
        }
        if (max_v > size)
            throw logic_error("Incorrect size. It must be higher than the highest node");

        initialization(size);
    }

    k2_tree_extended(k2_tree<2> tree, const size_t n_vertices) : k2_tree(tree)
    {
        initialization(n_vertices);
    }


//    void edge_iterator(function<int(uint, uint)> callback)
//    {
//        vector<ulong> pointerL;
//        pointerL.resize(max_level);
//        pointerL.push_back(0);
//        pointerL.push_back(k_k * k_k);
//
//        for (size_t i = 2; i <= max_level; i++)
//        {
//            //FIXME: REMOVE THESE get_* from k2_tree
//            ulong a = (get_rank_l(pointerL[i - 1] - 1) + 1) * k_k * k_k;
//            pointerL.push_back(a);
//        }
//
//        recursive_edge_iterator(pointerL, 0, 0, -1, -1, callback);
//    }

    k2_tree_extended unionOp(k2_tree &k_tree, uint n_vertices)
    {
        k2_tree<2> res = k_tree.unionOp(*this);
        return k2_tree_extended(res, n_vertices);
    }

    uint get_marked_edges()
    {
        return n_marked_edges;
    }

    void set_level(vector<int> div_level_table)
    {
        this->div_level_table = div_level_table;
    }

    bool mark_link_deleted(idx_type i, idx_type j)
    {
        if (k_t.size() == 0 && k_l.size() == 0)
            return false;
        size_type n = std::pow(k_k, k_height - 1);
        size_type k_2 = std::pow(k_k, 2);
        idx_type col, row;

        // This is duplicated to avoid an extra if at the loop. As idx_type
        // is unsigned and rank has an offset of one, is not possible to run
        // k_t_rank with zero as parameter at the first iteration.
        row = std::floor(i / static_cast<double>(n));
        col = std::floor(j / static_cast<double>(n));
        i = i % n;
        j = j % n;
        idx_type level = k_k * row + col;
        n = n / k_k;

        while (level < k_t.size())
        {
            if (k_t[level] == 0)
                return false;
            row = std::floor(i / static_cast<double>(n));
            col = std::floor(j / static_cast<double>(n));
            i = i % n;
            j = j % n;
            level = k_t_rank(level + 1) * k_2 + k_k * row + col;
            n = n / k_k;
        }

        if (k_l[level - k_t.size()] == 1)
        {
            k_l[level - k_t.size()] = 0;
            n_marked_edges++;
            return true;
        }
        return false;
    }
private:
    void initialization(uint size)
    {
        n_vertices = size;
        uint aux = log(n_vertices) / log(k_k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);

        for (size_t i = 0; i <= max_level; i++)
            div_level_table.push_back(pow(k_k, max_level - i));

        n_marked_edges = 0;
    }

    vector<int> div_level_table;
    uint n_marked_edges = 0;
    uint max_level;
    uint n_vertices;
};

#endif //K_TREE_EXTENDED_HPP
