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

    k2_tree_extended(vector<tuple<idx_type, idx_type>> &edges,
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

    k2_tree_extended(k2_tree<2> tree, uint n_vertices) : k2_tree(tree)
    {
        initialization(n_vertices);
    }

    uint get_level(uint i)
    {
        if (i < div_level_table.size())
            return div_level_table[i];
        return 0;
    }

    bool mark_link_deleted(uint x, uint y)
    {
        bool res = recursive_mark_deleted(x, y, 0, 0);
        cout << "RESULT FROM MARKED: " << res << endl;
        return res;
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
            ulong a = (get_rank_l(pointerL[i - 1] - 1) + 1) * k_k * k_k;
            pointerL.push_back(a);
        }

        recursive_edge_iterator(pointerL, 0, 0, -1, -1, callback);
    }

    bool check_link(uint x, uint y)
    {
        if (x >= n_vertices || y >= n_vertices)
            return false;

        return recursive_check_link_query(x, y, 0, 0);
    }

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

    uint recursive_mark_deleted(uint x, uint y, uint node, uint level)
    {
        int div_level = div_level_table[level];
        int newnode = x / div_level * k_k + y / div_level;
        newnode += node;

        if (k_l[newnode])
        {
            if (level < max_level - 1)
                return recursive_mark_deleted(x % div_level, y % div_level, k_l_rank(newnode) * k_k * k_k, level + 1);

            else
            {
                uint posInf;
                posInf = (k_l_rank(newnode)) * k_k * k_k;
                if (k_l[posInf + (y % k_k + (x % k_k) * k_k)])
                {
                    k_l[posInf + (y % k_k + (x % k_k) * k_k)] = 0;
                    n_marked_edges++;
                    return true;
                }
            }
        }
        return false;
    }

    bool recursive_check_link_query(uint p, uint q, uint node, uint level)
    {
        int div_level = div_level_table[level];

        int newnode = p / div_level * k_k + q / div_level;
        newnode += node;
        if (k_l[newnode])
        {
            if (level < max_level - 1)
                return recursive_check_link_query(p % div_level, q % div_level, k_l_rank(newnode) * k_k * k_k, level + 1);
            else
            {
                uint posInf;
                posInf = (k_l_rank(newnode)) * k_k * k_k;
                if (k_l[posInf + (q % k_k + (p % k_k) * k_k)])
                    return true;
            }
        }
        return false;
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
    uint n_marked_edges = 0;
    uint max_level;
    uint n_vertices;
};

#endif //K_TREE_EXTENDED_HPP
