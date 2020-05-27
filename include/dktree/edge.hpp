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
        os << "(" << e.x << ", " << e.y << ")";
        return os;
    }
};

class edge_node : public edge {
public:
    int x, y, next, prev;

    edge_node() {
        x = -1;
        y = -1;
        next = -1;
        prev = -1;
    }

    edge_node(int x, int y) : x(x), y(y) {
        next = -1;
        prev = -1;
    }

    bool operator==(const edge_node &rhs) const
    {
        return x == rhs.x && y == rhs.y && next == rhs.next && prev == rhs.prev;
    }

    bool operator!=(const edge_node &rhs) const
    {
        return !(*this == rhs);
    }

    friend ostream &operator<<(ostream &os, edge_node const &e) {
        os << e.next << "<- " << edge(e.x, e.y) << " ->" << e.prev << endl;
        return os;
    }
};

class edge_adj {
public:
    int next = -1;

    edge_adj() {}

    friend ostream &operator<<(ostream &os, edge_adj const &i) {
        os << "(" << i.next << ")";
        return os;
    }
};

#endif //IMPLEMENTATION_EDGE_HPP
