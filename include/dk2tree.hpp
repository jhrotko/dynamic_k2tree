#ifndef __D_K2TREE__
#define __D_K2TREE__

#include <array>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>

#include <sdsl/k2_tree.hpp>
#include "utils.hpp"
#include "edge_hash_table.hpp"
#include "adjacency_list.hpp"
#include "edge.hpp"
#include "k_tree_extended.hpp"

#define R 8

using namespace sdsl;
using namespace std;
using namespace k2_tree_ns;

//TODO: add generics
class dk2tree {
    typedef k2_tree_extended k2tree;

public:
    dk2tree(uint n_vertices) : n_vertices(n_vertices) {
        // Initialize Edge List
        const size_t max_edges = MAXSZ(n_vertices, 0);

        elements.resize(max_edges);
        n_elements = 0;

        edge_free.resize(max_edges);
        for (uint i = 0; i < max_edges; i++)
            edge_free[i] = i;

        //Initialize adjacency list
        adj_lst = adjacency_list(max_edges << 1);

        clean_free_lst();
        max_r = 0;
        for (size_t i = 0; i < R; i++) {
            // shared_ptr<k2tree> p(new k2tree(n_vertices));
            k_collection[i] = NULL;
        }
        // Initialize K2 tree Collections
        uint aux = log(n_vertices) / log(k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);

        for (size_t i = 0; i <= max_level; i++)
            div_level_table.push_back(pow(k, max_level - i));
    }

    void print() {
        cout << "------------------------" << endl;

        cout << "C0" << endl;
        cout << edge_lst << endl;

        for (size_t i = 0; i < k_collection.size(); i++) {
            cout << "C" << i + 1 << endl;
            if (k_collection_is_not_empty(i))
                k_collection[i]->print();
            else
                cout << "empty" << endl;
        }
        cout << "------------------------" << endl;
    }

    size_t size() {
        return n_total_edges;
    }

    void insert(uint x, uint y) {
        if (contains(x, y))
            return;

        size_t max_size = MAXSZ(max(n_vertices, n_total_edges), 0);
        if (n_elements < max_size) {
            insert_0(x, y);
            return;
        }

        size_t i = 0;
        for (; i < R; i++) {
            if (k_collection_is_not_empty(i))
                max_size += k_collection[i]->get_number_edges(); //TODO: rename me to size
            if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > max_size + 1)
                break;
        }

        if (i >= R)
            throw logic_error("Error: collection too big...");
        max_r = max(i, max_r);

        //Load edges in C0...
        vector<tuple<idx_type, idx_type>> free_edges;
        for (uint j = 0; j < n_elements; j++) {
            const tuple<idx_type, idx_type> e(
                    (idx_type) elements[j].x, (idx_type) elements[j].y);
            cout << elements[j].x << "  " << elements[j].y << endl;
            free_edges.push_back(e);
        }
        //Add new link...
        const tuple<idx_type, idx_type> e((idx_type) x, (idx_type) y);
        free_edges.push_back(e);
        assert(free_edges.size() == n_elements + 1);
        clean_C0();

        if (floor(log(n_vertices) / log(k)) == (log(n_vertices) / log(k)))
            max_level--;

        cout << "hue" << endl;
        shared_ptr<k2tree> tmp(new k2tree(free_edges, n_vertices));
        for (size_t j = 0; j <= i; j++) {
            if (k_collection_is_not_empty(j)) {
                k2tree aux = tmp->unionOp(*k_collection[j], n_vertices);
                tmp = make_shared<k2tree>(move(aux));
                tmp->set_level(div_level_table);
            }
            k_collection[j] = NULL;
        }

        assert(k_collection[i] == NULL);
        k_collection[i] = tmp;
        n_total_edges++;
    }

    bool contains(int x, int y) {
        //check in C0
        if (edge_lst.find(x, y) != -1)
            return true;

        // check in other containers
        for (size_t i = 0; i <= max_r; i++)
            if (k_collection_is_not_empty(i) && k_collection[i]->adj(x, y))
                return true;

        return false;
    }

    void erase(uint32 x, uint32 y) {
        size_t nodeIndex = edge_lst.find(x, y);
        if (nodeIndex != -1) {
            if (elements[nodeIndex].next != -1)
                elements[elements[nodeIndex].next].prev = elements[nodeIndex].prev;
            if (elements[nodeIndex].prev != -1)
                elements[elements[nodeIndex].prev].next = elements[nodeIndex].next;
            else
                adj_lst.insert(x, elements[nodeIndex].next);

            edge_free[elements.size()] = nodeIndex;
            n_total_edges--;
        } else {
            uint n_total_marked = 0;
            for (size_t l = 0; l <= max_r; l++) {

                cout << "l: ";
                if (k_collection_is_not_empty(l))
                    k_collection[l]->print();
                else
                    cout << "empty tree" << endl;

                if (k_collection_is_not_empty(l) && k_collection[l]->mark_link_deleted(x, y)) {
                    n_total_edges--;
                    // cout << "n_total_edges " << n_total_edges << endl;

                    uint k_marked_edges = k_collection[l]->get_marked_edges();
                    n_total_marked += k_marked_edges;

                    if (k_marked_edges == k_collection[l]->get_number_edges()) {
                        n_total_marked -= k_marked_edges;
                        delete &k_collection[l];
                        k_collection[l] = NULL;
                    }
                }
            }

            // cout << "RATIO " << ( n_total_edges / TAU(n_total_edges ))<< endl;
            // cout << "n_marked " << n_total_marked << endl;
            if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                /* Rebuild data structure... */
                array<shared_ptr<k2tree>, R> old_k_collection = k_collection;
                uint old_max_r = max_r;
                max_r = 0;

                for (size_t i = 0; i < R; i++) {
                    shared_ptr<k2tree> p(new k2tree(n_vertices));
                    k_collection[i] = p;
                }

                n_total_edges = edge_lst.size();
                for (size_t l = 0; l <= old_max_r; l++) {
                    if (old_k_collection[l] != NULL && old_k_collection[l] != 0) {
                        function<int(uint, uint)> func = [this](uint x, uint y) {
                            this->insert(x, y);
                            return 0;
                        };
                        old_k_collection[l]->edge_iterator(func);
                        delete &old_k_collection[l];
                        old_k_collection[l] = NULL;
                    }
                }
            }
        }
    }

    vector<int> list_neighbour(int x) {
        vector<int> neighbours;

        for (int index = adj_lst[x]; index != -1; index = elements[index].next)
            neighbours.push_back(elements[index].y);

        for (size_t l = 0; l <= max_r; l++)
            if (k_collection_is_not_empty(l)) {
                vector<k2_tree_ns::idx_type> lst = k_collection[l]->neigh(x);
                neighbours.insert(neighbours.end(), lst.begin(), lst.end()); //append
            }

        return neighbours;
    }

private:
    bool k_collection_is_not_empty(size_t i) {
        return k_collection[i] != NULL && k_collection[i] != 0;
    }

    void insert_0(uint x, uint y) {
        if (edge_lst.find(x, y)  == -1) {

            edgeNode newNode(x, y);
            if (adj_lst.find(x) != -1)
            {
                newNode.next = adj_lst[x];
                elements[adj_lst[x]].prev = n_elements;
                adj_lst.insert(x, n_elements);
            }

            edge_lst.insert(x, y, n_elements);
            elements[n_elements] = newNode;
            n_elements++;
            n_total_edges++;
        }
    }

    void clean_C0() {
        edge_lst.clear();
        clean_free_lst();
        adj_lst.clear();
        elements.clear();
        elements.resize(MAXSZ(n_vertices, 0));
        n_elements = 0;
    }

    void clean_free_lst() {
        for (size_t i = 0; i < edge_free.size(); i++)
            edge_free[i] = i;
    }

    uint max_level;
    uint max_r;
    uint n_vertices;
    uint n_total_edges = 0;

    const uint k = 2;
    // edge_lst replaces htable and elst;
    // where htable is the hash table with the dges and
    // the elst is the elements inside the edge hash table
    edge_hash_table edge_lst;
    vector<edgeNode> elements;
    int n_elements;
    vector<uint> edge_free; //should go inside hash_table

    adjacency_list adj_lst;
    array<shared_ptr<k2tree>, R> k_collection;
    vector<int> div_level_table;
};

#endif