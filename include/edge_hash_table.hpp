#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "edge.hpp"

using namespace std;

typedef unsigned int uint32;
typedef unsigned long uint64;

class edge_hash_table {
private:
    class Hash {
    public:
        uint32 operator()(edge const &e) const {
            uint64 key = edge_to_uint64(e);
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            return (uint32) key;
        }

        uint64 edge_to_uint64(edge const &e) const {
            uint64 concat_edge = e.x;
            concat_edge <<= 32;
            concat_edge |= e.y;

            return concat_edge;
        }
    };

    class Comparator {
    public:
        bool operator()(edge const &e1, edge const &e2) const {
            return e1.x == e2.x && e1.y == e2.y;
        }
    };

    typedef unordered_map<edge, int, Hash, Comparator> h_table;

protected:
    h_table ht;
public:

    edge_hash_table() = default;

    edge_hash_table(vector<edge> new_elements) {
        for (int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void insert(int x, int y, int index) {
        ht[edge(x, y)] = index;
    }

    // Returns the index of the index where the edge is.
    // Returns -1 in case it cannot find
    int find(int x, int y) {
    //TODO: FIXME: improve access, without iterator
        h_table::const_iterator iterator = ht.find(edge(x, y));
        if (iterator == ht.end())
            return -1;
        return iterator->second;
    }

    void erase(int x, int y)
    {
        ht.erase(edge(x,y));
    }

    size_t size() {
        return ht.size();
    }

    int &operator[](const edge &edge) {
        return ht[edge];
    }

    friend ostream &operator<<(ostream &os, edge_hash_table const &h) {
        for (auto item: h.ht)
            os << "[" << item.first << "] = " << item.second << "  ";
        os << endl;
        return os;
    }

    void clear()
    {
        ht.clear();
    }
};

#endif