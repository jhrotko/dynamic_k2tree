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
using namespace std;

class NodeEdge {
private:
    etype _next, _prev;
    bool _next_set, _prev_set;
public:


    NodeEdge() : _next_set(false), _prev_set(false) {}

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

    bool operator==(const NodeEdge &rhs) const
    {
        bool eval = _next_set == rhs._next_set && _prev_set == rhs._prev_set;

        if(eval) {
            if(_next_set)
                eval &= _next == rhs._next;
            if(_prev_set)
                eval &= _prev == rhs._prev;
        }
        return  eval;
    }

    bool operator!=(const NodeEdge &rhs) const
    {
        return !(*this == rhs);
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & _next_set;
        ar & _prev_set;
        ar & _next;
        ar & _prev;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int)
    {
        ar >> _next_set;
        ar >> _prev_set;
        ar >> _next;
        ar >> _prev;
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
            return e1 == e2;
        }
    };

protected:
    using h_table = unordered_map<Edge, unsigned int, Hash, Comparator>;
    h_table ht;
public:
//    friend class boost::serialization::access;
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

    void clear()
    {
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

    bool operator!=(const EdgeHashTable &rhs) const {
        return rhs.ht != ht;
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & ht;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int)
    {
        ar >> ht;
    }
};

#endif