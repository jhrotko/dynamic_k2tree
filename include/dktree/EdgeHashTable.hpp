#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <climits>

#include "../graph/Graph.hpp"

using namespace std;


class NodeEdge {
private:
    Edge _edge;
    etype _next, _prev;
    bool _next_set, _prev_set;
public:


    NodeEdge() : _next_set(false), _prev_set(false) {}

    NodeEdge(etype x, etype y) : _next_set(false), _prev_set(false) {
        _edge = Edge(x,y);
    }

    etype next() const {
        return _next;
    }

    void next(etype value) {
        _next = value;
        _next_set = true;
    }

    bool has_next() const {
        return _next_set;
    }

    etype prev() const {
        return _prev;
    }

    void prev(etype value) {
        _prev = value;
        _prev_set = true;
    }

    bool has_prev() const {
        return _prev_set;
    }

    etype x() const {
        return _edge.x();
    }

    etype y() const {
        return _edge.y();
    }

    bool operator==(const NodeEdge &rhs) const
    {
        return x() == rhs.x() && y() == rhs.y() && _next == rhs._next && _prev == rhs._prev;
    }
    bool operator!=(const NodeEdge &rhs) const
    {
        return !(*this == rhs);
    }
    friend ostream &operator<<(ostream &os, NodeEdge const &node) {
        string next_string = node._next_set? node._next + "<- ": "";
        string prev_string = node._prev_set? " ->" + node._prev :  "";
        os << "[" << next_string << node._edge << prev_string << "]" << endl;
        return os;
    }
};

class EdgeHashTable {
private:
    class Hash {
    public:
        unsigned int operator()(Edge const &e) const {
            unsigned long key = edge_to_uint64(e);
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            return (unsigned int) key;
        }

        unsigned long edge_to_uint64(Edge const &e) const {
            unsigned long concat_edge = e.x();
            concat_edge <<= 32;
            concat_edge |= e.y();

            return concat_edge;
        }
    };

    class Comparator {
    public:
        bool operator()(Edge const &e1, Edge const &e2) const {
            return e1.x() == e2.x() && e1.y() == e2.y();
        }
    };

    typedef unordered_map<Edge, unsigned int, Hash, Comparator> h_table;

protected:
    h_table ht;
public:

    EdgeHashTable() = default;

    EdgeHashTable(vector<Edge> new_elements) {
        for (unsigned int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void insert(etype x, etype y, unsigned int index) {
        ht[Edge(x, y)] = index;
    }

    // Returns the index of the index where the Edge is.
    // Returns UINT_MAX in case it cannot find
    unsigned int find(etype x, etype y) {
        h_table::const_iterator iterator = ht.find(Edge(x, y));
        if (iterator == ht.end())
            return UINT_MAX;
        return iterator->second;
    }

    bool contains(etype x, etype y) {
        return find(x,y) != UINT_MAX;
    }

    void erase(etype x, etype y)
    {
        ht.erase(Edge(x, y));
    }

    size_t size() {
        return ht.size();
    }

    unsigned int &operator[](const Edge &edge) {
        return ht[edge];
    }

    friend ostream &operator<<(ostream &os, EdgeHashTable const &h) {
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