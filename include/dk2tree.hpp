#ifndef __D_K2TREE__
#define __D_K2TREE__

#include <sdsl/k2_tree.hpp>
#include "./utils.hpp"
#include <list>
#include <map>
#include <array>

using namespace sdsl;


typedef struct edge
{
    uint x, y;
} edge;

//TODO: add generics
class dk2tree
{
    typedef k2_tree<2, bit_vector> k2t;

public:
    dk2tree(uint number_vertices)
    {
        // Initialize Edge List
        const uint max_edges = MAXSZ(number_vertices, 0);
        edge_lst.resize(max_edges);

        // Initialize Hash Table
        h_table.resize(max_edges << 1);

        //Initialize adjacency list
        adj_lst.resize(max_level << 1);
         
        // Initialize K2 tree Collections
        uint aux = log(number_vertices) / log(k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);
        div_level_table.resize(max_level);

        for (size_t i = 0; i < max_level; i++)
            div_level_table[i] = exp_pow(k, max_level-i);
    }

    void add_link(uint x, uint y)
    {    }

private:

    bool adj_k2_collection(uint x, uint y)
    {
        
        for (size_t i = 0; i < max_level; i++)
            if(k_collection[i].adj(x, y))
                return true;
        return false;
    }

    uint max_level;
    const uint k = 2;
    const uint r = 8;

    std::vector<edge> edge_lst;
    std::vector<uint> h_table;
    std::vector<uint> adj_lst;
    std::vector<k2t> k_collection;

    std::vector<uint> div_level_table;
};

#endif