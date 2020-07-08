//
// Created by joana on 24/05/20.
//

#ifndef _GRAPH_INTERFACE_HPP
#define _GRAPH_INTERFACE_HPP

#include <vector>
#include <iostream>

using namespace std;
using etype = int;

class Edge {
private:
    etype x_, y_;
public:
    Edge() {}

    Edge(etype x, etype y) : x_(x), y_(y) {}

    etype x() const {
        return x_;
    }

    etype y() const {
        return y_;
    }

    friend ostream &operator<<(ostream &os, Edge const &e) {
        os << "(" << e.x() << ", " << e.y() << ")";
        return os;
    }

    bool operator==(const Edge &rhs) const {
        return x() == rhs.x() && y() == rhs.y();
    }

    bool operator!=(const Edge &rhs) const {
        return !(*this == rhs);
    }
};

template<class IterImplement>
class GraphIterator {
public:
    virtual IterImplement &operator=(const IterImplement &lhs) = 0;
    virtual bool operator==(const IterImplement &rhs) const = 0;
    virtual bool operator!=(const IterImplement &rhs) const = 0;
    virtual IterImplement &operator++(int) = 0;
    virtual IterImplement &operator++() = 0;
};

template<class Implement>
class GraphEdgeIterator: public GraphIterator<Implement> {
public:
    virtual etype x() const = 0;
    virtual etype y() const = 0;
};

template<class Implement>
class GraphNodeIterator: public GraphIterator<Implement> {};

template<class GraphEdgeIteratorImpl, class GraphNodeIteratorImpl>
class Graph {
public:
    virtual void add_edge(etype x, etype y) = 0;

    virtual void del_edge(etype x, etype y) = 0;

    virtual vector<etype> list_neighbour(etype x) = 0;

    virtual size_t get_number_edges() const = 0;

    virtual size_t get_number_nodes() const = 0;

    virtual bool contains(etype x, etype y) = 0;

    virtual GraphEdgeIterator<GraphEdgeIteratorImpl> &edge_begin() = 0;

    virtual GraphEdgeIterator<GraphEdgeIteratorImpl> &edge_end() = 0;

    virtual GraphNodeIterator<GraphNodeIteratorImpl> &node_begin() = 0;

    virtual GraphNodeIterator<GraphNodeIteratorImpl> &node_end() = 0;
};

#endif //_GRAPH_INTERFACE_HPP
