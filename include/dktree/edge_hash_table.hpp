#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "../graph/Graph.hpp"

using namespace std;

using  uint64 = unsigned long;

class NodeDouble {
public:
    Edge edge;
    etype next, prev;

    NodeDouble() {
        edge = Edge(-1,-1);
        next = -1;
        prev = -1;
    }
    NodeDouble(etype x, etype y) {
        edge = Edge(x,y);
        next = -1;
        prev = -1;
    }
    etype x() const {
        return edge.x();
    }
    etype y() const {
        return edge.y();
    }

    bool operator==(const NodeDouble &rhs) const
    {
        return x() == rhs.x() && y() == rhs.y() && next == rhs.next && prev == rhs.prev;
    }
    bool operator!=(const NodeDouble &rhs) const
    {
        return !(*this == rhs);
    }
    friend ostream &operator<<(ostream &os, NodeDouble const &node) {
        os << node.next << "<- " << node.edge << " ->" << node.prev << endl;
        return os;
    }
};

class edge_hash_table {
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

    typedef unordered_map<Edge, int, Hash, Comparator> h_table;

protected:
    h_table ht;
public:

    edge_hash_table() = default;

    edge_hash_table(vector<Edge> new_elements) {
        for (int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void insert(etype x, etype y, unsigned int index) {
        ht[Edge(x, y)] = index;
    }

    // Returns the index of the index where the Edge is.
    // Returns -1 in case it cannot find
    int find(etype x, etype y) {
        h_table::const_iterator iterator = ht.find(Edge(x, y));
        if (iterator == ht.end())
            return -1;
        return iterator->second;
    }

    void erase(etype x, etype y)
    {
        ht.erase(Edge(x, y));
    }

    size_t size() {
        return ht.size();
    }

    int &operator[](const Edge &edge) {
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