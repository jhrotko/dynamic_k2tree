#ifndef __D_K2TREE__
#define __D_K2TREE__

#include <sdsl/k2_tree.hpp>
#include "./utils.hpp"
#include "./edge_hash_table.hpp"
#include "./adjacency_list.hpp"
#include <list>
#include <map>
#include <array>

#include <iostream>

using namespace sdsl;

//TODO: add generics
class dk2tree
{
    typedef k2_tree<2, bit_vector> k2tree;

public:
    dk2tree(uint n_vertices) : n_vertices(n_vertices)
    {
        // Initialize Edge List
        const size_t max_edges = MAXSZ(n_vertices, 0);
        edge_lst = edge_hash_table(max_edges);
        edge_free.resize(max_edges);

        //Initialize adjacency list
        adj_lst = adjacency_list(max_edges << 1);

        // Initialize K2 tree Collections
        uint aux = log(n_vertices) / log(k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);
        div_level_table.resize(max_level);
        for (size_t i = 0; i < max_level; i++)
            div_level_table[i] = exp_pow(k, max_level - i);

        clean_free_lst();
        k_collection.resize(r);
    }

    size_t size()
    {
        return edge_lst.size();
    }

    size_t n_edges()
    {
        return edge_lst.n_edges();
    }

    uint get_max_level()
    {
        return max_level;
    }

    void insert(uint x, uint y)
    {
        //TODO: Missing case where the edge already exists
        if(edge_lst.size() < MAXSZ(max(n_vertices, edge_lst.n_edges()), 0)) {
            insert_0(x,y);
            return;
        } 
        // uint n = MAXSZ(max(n_vertices, edge_lst.n_edges()), 0);
        // size_t i = 0;
        // for (; i < r; i++)
        // {
        //     n += k_collection[i]->get_number_edges(); //TODO: rename me to n_edges

        //     if (MAXSZ(max(n_vertices, edge_lst.n_edges()), i + 1) > n + 1)
        //         break;
        // }

        // if (i >= r)
        //     throw new std::logic_error("Error: collection too big...");

        // max_r = max(i, max_r);

        // vector<edge> free_edges;
        // for (size_t j = 0; j < edge_lst.size(); j++)
        //     free_edges.push_back(edge_lst[edge_free[j]]);
        // free_edges.push_back(edge(x, y));

        // uint k = edge_lst.size() + 1;
        // clean_C0(x, y);

        // cout << "here we go again" << endl;
        // uint max_level = floor(log(n_vertices) / log(k));
        // if (floor(log(n_vertices) / log(k)) == (log(n_vertices) / log(k)))
        //     max_level = max_level - 1;

        // //TODO: delete this translation later, ew
        // vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> convert_edges;
        // for (size_t j = 0; j < free_edges.size(); j++)
        // {
        //     const tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> e(
        //         (k2_tree_ns::idx_type)free_edges[j].x, (k2_tree_ns::idx_type)free_edges[j].y);
        //     convert_edges.push_back(e);
        // }

        // k2tree *tmp = new k2tree(convert_edges, free_edges.size());
        // for (size_t j = 0; j <= i; j++) //TODO: rename i
        // {
        //     if (k_collection[j] != NULL)
        //     {
        //         tmp = tmp->unionOp(*k_collection[j]);
        //         // tmp->div_level_table = div_level_table;
        //         delete k_collection[j];
        //     }
        //     k_collection[j] = NULL;
        // }
        // k_collection[i] = tmp;
    }

private:
    void insert_0(uint x, uint y)
    {
        if (!edge_lst.find(edge(x, y)))
        {
                std::cout << "wowow" << std::endl;
            //TODO: should go inside edge_hash_table
            if (edge_lst.n_edges() >= edge_lst.size_table())
            {
                uint32_t new_size = MAXSZ(max(n_vertices, edge_lst.n_edges()), 0);
                edge_lst.resize(new_size);
                edge_free.resize(new_size);
                for (size_t i = 0; i < new_size; i++)
                    edge_free[i] = i;
            }
            uint n_elements = edge_lst.size();
            uint i = edge_free[n_elements++];
            edge_lst.insert_element(edge_free, x, y);
            uint k = adj_lst.find(x);
            if (k != -1)
                adj_lst.insert(x, i);
            else
            {
                edge_lst[i].next = adj_lst[k].next;
                edge_lst[adj_lst[k].next].prev = i;
                adj_lst[k].next = i;
            }
            edge_lst.insert(i);
        }
    }

    void clean_C0(uint x, uint y)
    {
        edge_lst.clear();
        clean_free_lst();
        adj_lst.clear();
    }

    void clean_free_lst()
    {
        for (size_t i = 0; i < edge_lst.size(); i++)
            edge_free[i] = i;
    }

    uint max_level;
    uint max_r;
    uint n_vertices;

    const uint k = 2;
    const uint r = 8;
    // edge_lst replaces htable and elst;
    // where htable is the hash table with the dges and
    // the elst is the elements inside the edge hash table
    edge_hash_table edge_lst;
    std::vector<uint> edge_free; //should go inside hash_table

    adjacency_list adj_lst;
    std::vector<k2tree *> k_collection;

public:
    std::vector<uint> div_level_table; //TODO: I think this is not being really used
};

#endif