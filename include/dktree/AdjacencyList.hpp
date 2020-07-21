#ifndef __ADJACENCY_LIST_H__
#define __ADJACENCY_LIST_H__

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "../graph/Graph.hpp"

using namespace std;

class Node {
private:
    etype _next;
    bool _end = true;

public:
    Node() {}

    void next(etype value) {
        _next = value;
        _end = false;
    }

    etype next() const {
        return _next;
    }

    bool end() const{
        return _end;
    }

    bool operator==(const Node &node) const {
        return node._next == _next && node._end == _end;
    }

    friend ostream &operator<<(ostream &os, Node const &i) {
        if(i._end)
            os << "( )";
        else
            os << "(" <<  i._next << ")";
        return os;
    }
};

class AdjacencyList {

private:
    size_t n_elements = 0;
    size_t n_vertices;

    bool is_within_range(etype idx) const {
        return idx < n_vertices;
    }

public:
    vector<Node> vertices;
    AdjacencyList() {}

    AdjacencyList(size_t n_vertices) : n_vertices(n_vertices) {
        vertices.resize(n_vertices);
    }

    Node operator[](etype idx) {
        if(!is_within_range(idx))
            return Node();
        return vertices[idx];
    }

    void insert(etype node, etype next) {
        if(is_within_range(node)) {
            if(vertices[node].end())
                n_elements++;
            vertices[node].next(next);
        }
    }

    size_t size() {
        return n_elements;
    }

    void resize(size_t size)
    {
        n_vertices = size;
        vertices.resize(size);
    }

    void clear() {
        vertices.clear();
        vertices.resize(n_vertices);
        n_elements = 0;
    }

    void load(std::istream &in) {
        in.read((char*)&vertices, sizeof(vector<Node>));
        in.read((char*)&n_elements, sizeof(size_t));
        in.read((char*)&n_vertices, sizeof(size_t));
    }
};

#endif
