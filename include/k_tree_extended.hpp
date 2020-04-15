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


    void edge_iterator(function<int(uint, uint)> callback)
    {
        vector<ulong> pointerL;
        pointerL.resize(max_level);
        pointerL.push_back(0);
        pointerL.push_back(k_k * k_k);

        for (size_t i = 2; i <= max_level; i++)
        {
            //FIXME: REMOVE THESE get_* from k2_tree
            ulong a = (k_l_rank(pointerL[i - 1] - 1) + 1) * k_k * k_k;
            pointerL.push_back(a);
        }

        recursive_edge_iterator(pointerL, 0, 0, -1, -1, callback);
    }

    k2_tree_extended unionOp(k2_tree &k_tree, uint n_vertices)
    {
        k2_tree<2> res = k_tree.unionOp(*this);
        return k2_tree_extended(res, n_vertices);
    }

    void set_level(vector<int> div_level_table)
    {
        this->div_level_table = div_level_table;
    }
private:
    void initialization(uint size)
    {
        n_vertices = size;
        uint aux = log(n_vertices) / log(k_k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);

        for (size_t i = 0; i <= max_level; i++)
            div_level_table.push_back(pow(k_k, max_level - i));
    }

    void recursive_edge_iterator(vector<ulong> pointerL, uint dp, uint dq, uint x, int l, function<int(uint, uint)> proc)
    {
        uint y;
        if (l == max_level)
            if (k_l[x])
                proc(dp, dq);

        if ((l == max_level - 1) && (k_l[x]))
        {
            y = pointerL[l + 1];
            pointerL[l + 1] += k_k * k_k;
            for (size_t i = 0; i < k_k; i++)
                for (size_t j = 0; j < k_k; j++)
                    recursive_edge_iterator(pointerL, dp + i, dq + j, y + k_k * i + j, l + 1, proc);
        }
        if ((x == -1) || ((l < max_level - 1) && (k_l[x])))
        {
            y = pointerL[l + 1];
            pointerL[l + 1] += k_k * k_k;

            uint div_level = div_level_table[l + 1];
            for (size_t i = 0; i < k_k; i++)
                for (size_t j = 0; j < k_k; j++)
                    recursive_edge_iterator(pointerL, dp + div_level * i, dq + div_level * j, y + k_k * i + j, l + 1, proc);
        }
    }

    vector<int> div_level_table;

    uint max_level;
    uint n_vertices;
};

#endif //K_TREE_EXTENDED_HPP
