#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <climits>

#include "../graph/Graph.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/access.hpp>

#include <boost/serialization/unordered_map.hpp>

//using type = etype;
using type = uint32_t;

class NodeEdge {
private:
    type _x, _y, _next, _prev;
    bool _next_set, _prev_set;
public:
    NodeEdge() : _next_set(false), _prev_set(false) {}
    NodeEdge(type x, type y) : _x(x), _y(y), _next_set(false), _prev_set(false) {}

    type x() const {
        return _x;
    }

    void x(type value) {
        _x = value;
    }

    type y() const {
        return _y;
    }

    void y(type value) {
        _y = value;
    }

    type next() const {
        return _next;
    }

    void next(type value) {
        _next = value;
        _next_set = true;
    }

    bool has_next() const {
        return _next_set;
    }

    type prev() const {
        return _prev;
    }

    void prev(type value) {
        _prev = value;
        _prev_set = true;
    }

    bool has_prev() const {
        return _prev_set;
    }

    void remove_prev() {
        _prev_set = false;
    }

    void remove_next() {
        _next_set = false;
    }

    bool operator==(const NodeEdge &rhs) const {
        bool eval = _next_set == rhs._next_set && _prev_set == rhs._prev_set;

        if (eval) {
            if (_next_set)
                eval &= _next == rhs._next;
            if (_prev_set)
                eval &= _prev == rhs._prev;
        }
        return eval;
    }

    bool operator!=(const NodeEdge &rhs) const {
        return !(*this == rhs);
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & _x;
        ar & _y;
    }
};

class EdgeHashTable {
private:
    class Hash {
    public:
        unsigned int operator()(const NodeEdge &e) const {
            unsigned long key = edge_to_uint64(e);
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            return (unsigned int) key;
        }

        unsigned long edge_to_uint64(const NodeEdge &e) const {
            unsigned long concat_edge = e.x();
            concat_edge <<= 32;
            concat_edge |= e.y();

            return concat_edge;
        }
    };

    class Comparator {
    public:
        bool operator()(NodeEdge const &e1, NodeEdge const &e2) const {
            return e1 == e2;
        }
    };

protected:
    using h_table = unordered_map<NodeEdge, unsigned int, Hash, Comparator>;
    h_table ht;
public:
//    friend class boost::serialization::access;
    EdgeHashTable() = default;

    EdgeHashTable(vector<NodeEdge> &new_elements) {
        for (unsigned int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void insert(etype x, etype y, unsigned int index) {
        ht[NodeEdge(x, y)] = index;
    }

    void reserve(uint size) {
        ht.reserve(size);
    }

    // Returns the index of the index where the NodeEdge is.
    // Returns -1 in case it cannot find
    int64_t find(etype x, etype y) const {
        h_table::const_iterator iterator = ht.find(NodeEdge(x, y));
        if (iterator == ht.end())
            return -1;
        return iterator->second;
    }

    bool contains(etype x, etype y) const {
        return find(x, y) != -1;
    }

    void erase(etype x, etype y) {
        ht.erase(NodeEdge(x, y));
    }

    size_t size() {
        return ht.size();
    }

    unsigned int &operator[](const NodeEdge &edge) {
        return ht[edge];
    }

    void clear() {
        ht.clear();
    }

    h_table::const_iterator cbegin() const noexcept {
        return ht.cbegin();
    }

    h_table::const_iterator cend() const noexcept {
        return ht.cend();
    }

    bool operator==(const EdgeHashTable &rhs) const {
        return rhs.ht == ht;
    }

    bool operator!=(const EdgeHashTable &edg_ht) {
        return !(*this == edg_ht);
    }
};

#endif