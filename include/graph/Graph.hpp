//
// Created by joana on 24/05/20.
//

#ifndef _GRAPH_INTERFACE_HPP
#define _GRAPH_INTERFACE_HPP

#include <vector>

using namespace std;

class Graph {
public:
    virtual void add_edge(unsigned int x, unsigned int y) = 0;
    virtual void del_edge(unsigned int x, unsigned int y) = 0;
    virtual vector<int> list_neighbour(unsigned int x) = 0;
    virtual size_t get_number_edges() const = 0;
    virtual size_t get_number_nodes() const = 0;
    virtual bool contains(unsigned int x, unsigned int y) = 0;
};

template<class Iter>
class GraphIterable : public Graph {
public:
    virtual Iter& edge_begin() = 0;
    virtual Iter& edge_end() = 0;
};

#endif //_GRAPH_INTERFACE_HPP
