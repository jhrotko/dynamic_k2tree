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
#include <functional>

#include <iostream>

#define R 8

using namespace sdsl;

class k2_tree_extended : public k2_tree<2> {
public:
    k2_tree_extended(uint n_vertices) : k2_tree() {
        initialization(n_vertices);
    }

    k2_tree_extended(std::vector<std::tuple<idx_type, idx_type>> &edges,
                     const size_type size) : k2_tree(edges, size) {
        uint max_v = 0;

        for (size_t i = 0; i < edges.size(); i++) {
            max_v = max(max_v, std::get<0>(edges[i]));
            max_v = max(max_v, std::get<1>(edges[i]));
        }
        if (max_v > size)
            throw std::logic_error("Incorrect size. It must be higher than the highest node");

        initialization(size);
    }

    k2_tree_extended(k2_tree<2> tree, uint n_vertices) : k2_tree(tree) {
        initialization(n_vertices);
    }

    uint get_level(uint i) {
        if (i < div_level_table.size())
            return div_level_table[i];
        return 0;
    }

    bool mark_link_deleted(uint x, uint y) {
        bool res = recursive_mark_deleted(x, y, 0, 0);
        std::cout << "RESULT FROM MARKED: " << res << std::endl;
        return res;
    }

    void edge_iterator(std::function<int(uint, uint)> callback) {
        std::vector<ulong> pointerL;
        pointerL.resize(max_level);
        pointerL.push_back(0);
        pointerL.push_back(get_k() * get_k());

        for (size_t i = 2; i <= max_level; i++) {
            ulong a = (get_rank_l(pointerL[i - 1] - 1) + 1) * get_k() * get_k();
            pointerL.push_back(a);
        }

        recursive_edge_iterator(pointerL, 0, 0, -1, -1, callback);
    }

    bool check_link(uint x, uint y) {
        if (x >= n_vertices || y >= n_vertices)
            return false;

        return recursive_check_link_query(x, y, 0, 0);
    }

    k2_tree_extended unionOp(k2_tree<2> &k_tree, uint n_vertices) {
        k2_tree<2> unionTree = k_tree.unionOp(*this);
        return k2_tree_extended(unionTree, n_vertices);
    }

    uint get_marked_edges() {
        return n_marked_edges;
    }

    void set_div_level_table(std::vector<uint> &table) {
        this->div_level_table = table;
    }

    void print() {
        std::cout << "  t: ";
        for (int i = 0; i < k_t.size(); i++) {
            std::cout << k_t[i];
        }
        cout << endl;

        std::cout << "  l: ";
        for (int i = 0; i < k_l.size(); i++) {
            std::cout << k_l[i];
        }
        cout << endl << "  n edges: " << get_number_edges() << endl;
    }

private:
    void initialization(uint size) {
        n_vertices = size;
        uint aux = log(n_vertices) / log(k_k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);
        for (size_t i = 0; i < R; i++) {
            std::vector<uint> div_table;
            div_level_table.resize(max_level + 1);

            for (size_t j = 0; j <= max_level; j++)
                div_level_table[j] = pow(k_k, max_level - j);
        }
        n_marked_edges = 0;
    }

    bool recursive_mark_deleted(uint x, uint y, uint node, uint level) {
        int div_level = div_level_table[level];
        int newnode = (x / div_level * k_k + y / div_level) + node;

        std::cout << "x: " << x << "  y: " << y << std::endl;
        std::cout << "div_level:  " << div_level << std::endl;
        std::cout << "newnode: " << newnode << std::endl;

        print();

        if (get_t_l(newnode)) {
            std::cout << "level:  " << level << std::endl;
            std::cout << "max_level:  " << max_level << std::endl;
            if (level < max_level - 1) {
                cout <<  rank_t_l(newnode) << endl;
                return recursive_mark_deleted(x % div_level, y % div_level, rank_t_l(newnode) * k_k * k_k, level + 1);
            }

            else {

                uint posInf = rank_t_l(newnode) * k_k * k_k;
                std::cout << "posInf: " << posInf << std::endl;
                std::cout << "posInf: " << posInf + (y % k_k + (x % k_k) * k_k) << std::endl;
                if (get_t_l(posInf + (y % k_k + (x % k_k) * k_k))) {
                    clean_t_l(posInf + (y % k_k + (x % k_k) * k_k));
                    n_marked_edges++;
                    return true;
                }
            }
        }
        return false;
    }

    bool recursive_check_link_query(uint p, uint q, uint node, uint level) {
        int div_level = div_level_table[level];
        uint K = get_k();
        int newnode = p / div_level * K + q / div_level;
        newnode += node;
        if (get_t_l(newnode)) {
            if (level < max_level - 1)
                return recursive_check_link_query(p % div_level, q % div_level, get_rank_l(newnode) * K * K, level + 1);
            else {
                uint posInf;
                posInf = (get_rank_l(newnode)) * K * K;
                if (get_l()[posInf + (q % K + (p % K) * K)])
                    return true;
            }
        }
        return false;
    }

    void recursive_edge_iterator(std::vector<ulong> pointerL, uint dp, uint dq, uint x, int l,
                                 std::function<int(uint, uint)> proc) {
        uint y;
        if (l == max_level)
            if (get_l()[x])
                proc(dp, dq);

        if ((l == max_level - 1) && (get_l()[x])) {
            y = pointerL[l + 1];
            pointerL[l + 1] += get_k() * get_k();
            for (size_t i = 0; i < get_k(); i++)
                for (size_t j = 0; j < get_k(); j++)
                    recursive_edge_iterator(pointerL, dp + i, dq + j, y + get_k() * i + j, l + 1, proc);
        }
        if ((x == -1) || ((l < max_level - 1) && (get_l()[x]))) {
            y = pointerL[l + 1];
            pointerL[l + 1] += get_k() * get_k();

            uint div_level = div_level_table[l + 1];
            for (size_t i = 0; i < get_k(); i++)
                for (size_t j = 0; j < get_k(); j++)
                    recursive_edge_iterator(pointerL, dp + div_level * i, dq + div_level * j, y + get_k() * i + j,
                                            l + 1, proc);
        }
    }

    int get_t_l(int index) {
        if (k_t.size() > 0) {
            if (index < k_t.size())
                return k_t[index];
            return k_l[index - k_t.size()-1];
        }
        return k_l[index];
    }

    int rank_t_l(int index) {
        if (k_t.size() > 0) {
            if (index < k_t.size())
                return k_t_rank(index+1);
            return k_l_rank(index+1 - k_t.size()) + k_t_rank(k_t.size());
        }
        return k_l_rank(index+1);
    }

    void clean_t_l(int index)
    {
        if (k_t.size() > 0) {
            if (index < k_t.size())
                k_t[index] = 0;
            k_l[index - k_t.size()-1] = 0;
        }
        k_l[index] = 0;
    }

    bool is(uint node) {
        return k_l[node] == 1;
    }

    uint n_marked_edges = 0;
    uint max_level;
    uint n_vertices;
    std::vector<uint> div_level_table;

};

//TODO: add generics
class dk2tree {
    typedef k2_tree_extended k2tree;

public:
    dk2tree(uint n_vertices) : n_vertices(n_vertices) {
        // Initialize Edge List
        const size_t max_edges = MAXSZ(n_vertices, 0);
        edge_lst = edge_hash_table(max_edges);
        edge_free.resize(max_edges);
        for (uint i = 0; i < max_edges; i++)
            edge_free[i] = i;

        //Initialize adjacency list
        adj_lst = adjacency_list(max_edges << 1);

        clean_free_lst();
        max_r = 0;
        for (size_t i = 0; i < R; i++) {
            std::shared_ptr<k2tree> p(new k2tree(n_vertices));
            k_collection[i] = p;
        }
        // Initialize K2 tree Collections
        uint aux = log(n_vertices) / log(k);
        max_level = floor(aux) == aux ? floor(aux) - 1 : floor(aux);
    }

    void print() {
        for (size_t i = 0; i < k_collection.size(); i++)
            std::cout << "i: " << i << "    value: " << k_collection[i] << std::endl;
    }

    size_t size() {
        return n_total_edges;
    }

    uint get_max_level() {
        return max_level;
    }

    void insert(uint x, uint y) {
        if (contains(x, y)) return;
        cout << "what" << endl;

        if (edge_lst.n_edges() < MAXSZ(max(n_vertices, n_total_edges), 0)) {
            insert_0(x, y);
            return;
        }
        uint n = MAXSZ(max(n_vertices, n_total_edges), 0);
        size_t i = 0;
        for (; i < R; i++) {
            if (k_collection[i] != NULL)
                n += k_collection[i]->get_number_edges(); //TODO: rename me to n_edges

            if (MAXSZ(max(n_vertices, n_total_edges), i + 1) > n + 1)
                break;
        }

        if (i >= R)
            throw std::logic_error("Error: collection too big...");

        vector<edge> free_edges;
        for (uint k = 0; k < edge_lst.n_edges(); k++)
            free_edges.push_back(edge_lst[edge_free[k]]);

        free_edges.push_back(edge(x, y));
        assert(free_edges.size() == edge_lst.n_edges() + 1);
        clean_C0(x, y);

        if (floor(log(n_vertices) / log(k)) == (log(n_vertices) / log(k)))
            max_level = max_level - 1;

        max_r = max(i, max_r);
        //TODO: delete this translation later, ew
        vector<tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type>> convert_edges;
        for (size_t j = 0; j < free_edges.size(); j++) {
            const tuple<k2_tree_ns::idx_type, k2_tree_ns::idx_type> e(
                    (k2_tree_ns::idx_type) free_edges[j].x, (k2_tree_ns::idx_type) free_edges[j].y);
            convert_edges.push_back(e);
            cout << "edge  x: " << std::get<0>(e) << " y: " << std::get<1>(e) << endl;
        }

        std::shared_ptr<k2tree> tmp(new k2tree(convert_edges, n_vertices));
        for (size_t j = 0; j <= i; j++) {
            if (k_collection_is_not_empty(j)) {
                cout << "before union" << endl;
                tmp->print();
                k_collection[j]->print();

                k2tree aux = tmp->unionOp(*k_collection[j], n_vertices);
                tmp = std::make_shared<k2tree>(std::move(aux));

                cout << "after union" << endl;
                tmp->print();
                k_collection[j]->print();
            }
            k_collection[j] = NULL;
        }

        cout << "INSERTED IN INDEX " << i << endl;
        k_collection[i] = tmp;
        n_total_edges++;
    }

    bool contains(int x, int y) {
        //check in CO
        if (edge_lst.find(edge(x, y)) != -1)
            return true;
         cout << "nani" << endl;
        // check in other containers
        for (size_t i = 0; i <= max_r; i++)
            if (k_collection_is_not_empty(i) && k_collection[i]->adj(x, y))
                return true;

        return false;
    }

    void erase(uint32 x, uint32 y) {
        int index = edge_lst.find(edge(x, y));
        if (index != -1) {
            edge_lst.erase(index);
            if (edge_lst[index].next > 0)
                edge_lst[edge_lst[index].next].prev = edge_lst[index].prev;

            if (edge_lst[index].prev > 0)
                edge_lst[edge_lst[index].prev].next = edge_lst[index].next;
            else {
                int l = adj_lst.find(x);
                adj_lst[l].next = edge_lst[index].next;
            }

            edge_free[edge_lst.n_edges()] = index;
            n_total_edges--;
        } else {
            cout << "what" << endl;
            uint n_total_marked = 0;
            for (size_t l = 0; l <= max_r; l++) {
                if (k_collection_is_not_empty(l)) {
                    k_collection[l]->print();

                    if (k_collection[l]->mark_link_deleted(x, y)) {
                        n_total_edges--;
                        std::cout << "n_total_edges " << n_total_edges << std::endl;

                        uint k_marked_edges = k_collection[l]->get_marked_edges();
                        n_total_marked += k_marked_edges;

                        if (k_marked_edges == k_collection[l]->get_number_edges()) {
                            n_total_marked -= k_marked_edges;
                            delete &k_collection[l];
                            k_collection[l] = NULL;
                        }
                    }
                }
            }

            std::cout << "RATIO " << (n_total_edges / TAU(n_total_edges)) << std::endl;
            std::cout << "n_marked " << n_total_marked << std::endl;
            if (n_total_marked > n_total_edges / TAU(n_total_edges)) {
                /* Rebuild data structure... */
                std::array<std::shared_ptr<k2tree>, R> old_k_collection = k_collection;
                uint old_max_r = max_r;
                max_r = 0;

                for (size_t i = 0; i < R; i++) {
                    std::shared_ptr<k2tree> p(new k2tree(n_vertices));
                    k_collection[i] = p;
                }

                n_total_edges = edge_lst.n_edges();
                for (size_t l = 0; l <= old_max_r; l++) {
                    if (old_k_collection[l] != NULL && old_k_collection[l] != 0) {
                        std::function<int(uint, uint)> func = [this](uint x, uint y) {
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

    std::vector<int> list_neighbour(int x) {
        std::vector<int> n;
        int index = adj_lst.find(x);
        if (index != -1)
            for (index = adj_lst[index].next; index != EMPTY; index = edge_lst[index].next)
                n.push_back(edge_lst[index].y);

        for (size_t l = 0; l <= max_r; l++)
            if (k_collection_is_not_empty(l)) {
                std::vector<k2_tree_ns::idx_type> lst = k_collection[l]->neigh(x);
                n.insert(n.end(), lst.begin(), lst.end()); //append
            }

        return n;
    }

private:
    bool k_collection_is_not_empty(size_t i) {
        return k_collection[i] != NULL && k_collection[i] != 0 && k_collection[i]->get_number_edges() != 0;
    }

    void insert_0(uint x, uint y) {
        if (edge_lst.find(edge(x, y)) < 0) {
            //TODO: should go inside edge_hash_table
            if (edge_lst.n_edges() >= edge_lst.size_table()) {
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
            else {
                edge_lst[i].next = adj_lst[k].next;
                edge_lst[adj_lst[k].next].prev = i;
                adj_lst[k].next = i;
            }
            edge_lst.insert(i);
            n_total_edges++;
        }
    }

    void clean_C0(uint x, uint y) {
        edge_lst.clear();
        clean_free_lst();
        adj_lst.clear();
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
    std::vector<uint> edge_free; //should go inside hash_table

    adjacency_list adj_lst;
    std::array<std::shared_ptr<k2tree>, R> k_collection;
};

#endif