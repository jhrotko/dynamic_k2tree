#ifndef __D_K2TREE__
#define __D_K2TREE__

#include <sdsl/k2_tree.hpp>
#include "./utils.hpp"
#include "./edge_hash_table.hpp"
#include "./adjacency_list.hpp"
#include <list>
#include <map>
#include <array>
#include <memory>

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
        for (uint i = 0; i < max_edges; i++)
            edge_free[i] = i;

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
        k_collection.assign(r, NULL);
        for (size_t i = 0; i < r; i++)
        {
            std::shared_ptr<k2tree> p(new k2tree());
            k_collection[i] = p;
        }
    }

    void print()
    {
        for (size_t i = 0; i < k_collection.size(); i++)
            std::cout << "i: " << i << "    value: " << k_collection[i] << std::endl;
    }

    size_t size()
    {
        return n_total_edges;
    }

    uint get_max_level()
    {
        return max_level;
    }

    void insert(uint x, uint y)
    {
        //TODO: Missing case where the edge already exists
        if (edge_lst.n_edges() < MAXSZ(max(n_vertices, n_total_edges), 0))
        {
            insert_0(x, y);
            return;
        }
        uint n = MAXSZ(max(n_vertices, n_total_edges), 0);
        size_t i = 0;
        for (; i < r; i++)
        {
            if (k_collection[i] != NULL)
                n += k_collection[i]->get_number_edges(); //TODO: rename me to n_edges

            if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > n + 1)
                break;
        }

        if (i >= r)
            throw new std::logic_error("Error: collection too big...");

        vector<edge> free_edges;
        for (uint k = 0; k < edge_lst.n_edges(); k++)
            free_edges.push_back(edge_lst[edge_free[k]]);

        free_edges.push_back(edge(x, y));
        assert(free_edges.size() == edge_lst.n_edges() + 1);
        clean_C0(x, y);

        uint max_level = floor(log(n_vertices) / log(k));
        if (floor(log(n_vertices) / log(k)) == (log(n_vertices) / log(k)))
            max_level = max_level - 1;

        //TODO: delete this translation later, ew
        vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> convert_edges;
        for (size_t j = 0; j < free_edges.size(); j++)
        {
            const tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> e(
                (k2_tree_ns::idx_type)free_edges[j].x, (k2_tree_ns::idx_type)free_edges[j].y);
            convert_edges.push_back(e);
        }
        std::shared_ptr<k2tree> tmp(new k2tree(convert_edges, free_edges.size()));
        for (size_t j = 0; j <= i; j++)
        {
            if (k_collection[j] != NULL || (k_collection[j] != 0 && k_collection[j]->get_number_edges() == 0))
            {
                k2tree aux = tmp->unionOp(*k_collection[j]);
                tmp = std::make_shared<k2tree>(std::move(aux));
            }
            k_collection[j] = NULL;
        }
        k_collection[i] = tmp;
        n_total_edges++;
    }

private:
    void insert_0(uint x, uint y)
    {
        if (edge_lst.find(edge(x, y)) < 0)
        {
            //TODO: should go inside edge_hash_table
            if (edge_lst.n_edges() >= edge_lst.size_table())
            {
                uint32_t new_size = MAXSZ(max(n_vertices, n_total_edges), 0);
                edge_lst.resize(new_size);
                edge_free.resize(new_size);
                for (size_t i = 0; i < new_size; i++)
                    edge_free[i] = i;
            }
            uint n_elements = edge_lst.n_edges();
            uint i = edge_free[n_elements++];
            edge_lst.change_element(i, x, y);
            int k = adj_lst.find(x);
            if (k == -1)
                adj_lst.insert(x, i);
            else
            {
                edge_lst[i].next = adj_lst[k].next;
                edge_lst[adj_lst[k].next].prev = i;
                adj_lst[k].next = i;
            }
            edge_lst.insert(i);
            n_total_edges++;
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
        for (size_t i = 0; i < edge_free.size(); i++)
            edge_free[i] = i;
    }

    uint max_level;
    uint max_r;
    uint n_vertices;
    uint n_total_edges = 0;

    const uint k = 2;
    const uint r = 8;
    // edge_lst replaces htable and elst;
    // where htable is the hash table with the dges and
    // the elst is the elements inside the edge hash table
    edge_hash_table edge_lst;
    std::vector<uint> edge_free; //should go inside hash_table

    adjacency_list adj_lst;
    std::vector<std::shared_ptr<k2tree>> k_collection;

public:
    std::vector<uint> div_level_table; //TODO: I think this is not being really used
};

#endif