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
//    tuple<etype, etype> _edge;
    etype _next, _prev;
    bool _next_set, _prev_set;
public:


    NodeEdge() : _next_set(false), _prev_set(false) {}

//    NodeEdge(etype x, etype y) : _next_set(false), _prev_set(false) {
////        _edge = tuple<etype, etype>(x,y);
//    }

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

//    etype x() const {
//        return get<0>(_edge);
//    }
//
//    etype y() const {
//        return get<1>(_edge);
//    }

    bool operator==(const NodeEdge &rhs) const
    {
        bool eval = _next_set == rhs._next_set && _prev_set == rhs._prev_set;

        return  eval && (_prev_set && rhs._prev_set && _prev == rhs._prev) &&
        (_next_set && rhs._next_set && _next == rhs._next)&& _next == rhs._next;
    }
    bool operator!=(const NodeEdge &rhs) const
    {
        return !(*this == rhs);
    }
//    friend ostream &operator<<(ostream &os, NodeEdge const &node) {
//        string next_string = node._next_set? node._next + "<- ": "";
//        string prev_string = node._prev_set? " ->" + node._prev :  "";
//        os << "[" << next_string << node.x() << ", " << node.y() << prev_string << "]" << endl;
//        return os;
//    }
};

class EdgeHashTable {
private:
    class Hash {
    public:
        unsigned int operator()(tuple<etype, etype> const &e) const {
            unsigned long key = edge_to_uint64(e);
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            return (unsigned int) key;
        }

        unsigned long edge_to_uint64(tuple<etype, etype> const &e) const {
            unsigned long concat_edge = get<0>(e);
            concat_edge <<= 32;
            concat_edge |= get<1>(e);

            return concat_edge;
        }
    };

    class Comparator {
    public:
        bool operator()(tuple<etype, etype> const &e1, tuple<etype, etype> const &e2) const {
            return get<0>(e1) == get<0>(e2) && get<1>(e1) == get<1>(e2);
        }
    };

protected:
    using h_table = unordered_map<tuple<etype, etype>, unsigned int, Hash, Comparator>;
    h_table ht;
public:
    friend class boost::serialization::access;
    EdgeHashTable() = default;

    EdgeHashTable(vector<tuple<etype, etype>> new_elements) {
        for (unsigned int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void insert(etype x, etype y, unsigned int index) {
        ht[tuple<etype, etype>(x, y)] = index;
    }

    // Returns the index of the index where the Edge is.
    // Returns UINT_MAX in case it cannot find
    unsigned int find(etype x, etype y) {
        h_table::const_iterator iterator = ht.find(tuple<etype, etype>(x, y));
        if (iterator == ht.end())
            return UINT_MAX;
        return iterator->second;
    }

    bool contains(etype x, etype y) {
        return find(x,y) != UINT_MAX;
    }

    void erase(etype x, etype y)
    {
        ht.erase(tuple<etype, etype>(x, y));
    }

    size_t size() {
        return ht.size();
    }

    unsigned int &operator[](const tuple<etype, etype> &edge) {
        return ht[edge];
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<EdgeHashTable>(*this);
        ar & ht;
    }

//    Map deserialize(Deserializer& in) {
//        +   Map::key_compare comparator;
//        +   Map::allocator_type allocator;
//        +
//                +   in >> comparator >> allocator;
//        +
//                +   Map map(comparator, allocator)
//
//        size_t size = 0;
//        in >> size;
//        for (size_t i = 0; i != size; ++i) {
//            Map::key_type key;
//            Map::mapped_type value;
//            in >> key >> value;
//            map[key] = value;
//        }
//
//        return map;
//    }

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
};

#endif