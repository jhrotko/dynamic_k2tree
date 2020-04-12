#ifndef __ADJACENCY_LIST_H__
#define __ADJACENCY_LIST_H__

#include <vector>
#include <iostream>

#include "utils.hpp"
#include "edge.hpp"

using namespace std;

class adjacency_list {
private:
    vector<adjEdge> vertices;
    size_t n_elements = 0;
    size_t n_vertices;

public:
    adjacency_list() {}

    adjacency_list(size_t n_vertices) : n_vertices(n_vertices) {
        vertices.resize(n_vertices);
    }

    int &operator[](size_t idx) {
        return vertices[idx].next;
    }

    void insert(int node, int next) {
        if(vertices[node].next == -1)
            n_elements++;
        vertices[node].next = next;
    }

    //might bring problems later because it returns the first index that it finds
    int find(int node) {
        return vertices[node].next == -1 ? -1 : vertices[node].next;
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
