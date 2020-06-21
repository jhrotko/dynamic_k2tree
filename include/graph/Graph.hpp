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
};

template<class Implement>
class GraphEdgeIterator {
public:
    virtual etype x() const = 0;
    virtual etype y() const = 0;

    virtual bool operator!=(const Implement &rhs) const = 0;
    virtual Implement &operator++(int) = 0;
};

template<class GraphEdgeIteratorImpl>
class Graph {
public:
    virtual void add_edge(unsigned int x, unsigned int y) = 0;
    virtual void del_edge(unsigned int x, unsigned int y) = 0;

    virtual vector<int> list_neighbour(unsigned int x) = 0; //TODO: change int to etype
    virtual size_t get_number_edges() const = 0;
    virtual size_t get_number_nodes() const = 0;
    virtual bool contains(unsigned int x, unsigned int y) = 0;
    virtual GraphEdgeIterator<GraphEdgeIteratorImpl> &edge_begin() = 0;
    virtual GraphEdgeIterator<GraphEdgeIteratorImpl> &edge_end() = 0;
};

#endif //_GRAPH_INTERFACE_HPP
