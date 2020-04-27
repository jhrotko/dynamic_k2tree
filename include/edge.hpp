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

class EdgeNode : public edge {
public:
    int x, y, next, prev;

    EdgeNode() {
        x = -1;
        y = -1;
        next = -1;
        prev = -1;
    }

    EdgeNode(int x, int y) : x(x), y(y) {
        next = -1;
        prev = -1;
    }

    friend ostream &operator<<(ostream &os, EdgeNode const &e) {
        os << e.next << "<- (" << edge(e.x, e.y) << ") ->" << e.prev << endl;
        return os;
    }
};

class AdjEdge {
public:
    int next = -1;

    AdjEdge() {}

    friend ostream &operator<<(ostream &os, AdjEdge const &i) {
        os << "(" << i.next << ")" << endl;
        return os;
    }
};

#endif //IMPLEMENTATION_EDGE_HPP
