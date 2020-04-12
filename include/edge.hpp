#ifndef IMPLEMENTATION_EDGE_HPP
#define IMPLEMENTATION_EDGE_HPP

#include <iostream>
#include <memory>

using namespace std;

class edge {
public:
    int x, y;

    edge() {}

    edge(int x, int y) : x(x), y(y) {}

    edge operator()() const {
        return *this;
    }

    friend ostream &operator<<(ostream &os, edge const &e) {
        os << "(" << e.x << ", " << e.y << ")" << endl;
        return os;
    }
};

class edgeNode : public edge {
public:
    int x, y, next, prev;

    edgeNode() {
        x = -1;
        y = -1;
        next = -1;
        prev = -1;
    }

    edgeNode(int x, int y) : x(x), y(y) {
        next = -1;
        prev = -1;
    }

    friend ostream &operator<<(ostream &os, edgeNode const &e) {
        os << e.next << "<- (" << edge(e.x,e.y) << ") ->" << e.prev << endl;
        return os;
    }
};

class adjEdge {
public:
    int next = -1;

    adjEdge() {}

    friend ostream &operator<<(ostream &os, adjEdge const &i) {
        os << "(" << i.next << ")" << endl;
        return os;
    }
};

#endif //IMPLEMENTATION_EDGE_HPP
