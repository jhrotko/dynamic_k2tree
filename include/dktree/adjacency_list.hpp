#ifndef __ADJACENCY_LIST_H__
#define __ADJACENCY_LIST_H__

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "../graph/Graph.hpp"

using namespace std;

class Node {
public:
    etype next = -1;
    Node() {}

    friend ostream &operator<<(ostream &os, Node const &i) {
        os << "(" << i.next << ")";
        return os;
    }
};

class adjacency_list {

private:
    size_t n_elements = 0;
    size_t n_vertices;

public:
    vector<Node> vertices;
    adjacency_list() {}

    adjacency_list(size_t n_vertices) : n_vertices(n_vertices) {
        vertices.resize(n_vertices);
    }

    int operator[](size_t idx) {
        if (idx > n_vertices)
            return -1;
        return vertices[idx].next;
    }

    void insert(unsigned int node, etype next) {
        if(vertices[node].next == -1)
            n_elements++;
        vertices[node].next = next;
    }

    uint size() {
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
};

#endif
