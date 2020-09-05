//
// Created by joana on 04/09/20.
//

#ifndef DYNAMIC_K2TREE_CONTAINER_0_NEIGH_ITERATOR_HPP
#define DYNAMIC_K2TREE_CONTAINER_0_NEIGH_ITERATOR_HPP

#include <iterator>

namespace dynamic_ktree {

    template<class Container_0>
    class Container_0_neigh_iterator {
    private:
        const Container_0 *_C0;
        int64_t _ptr;
        size_t k;
    public:
        using value_type = int64_t;
        using pointer = int64_t*;
        using reference = int64_t &;
        using iterator_category = std::forward_iterator_tag;

        Container_0_neigh_iterator() {}

        Container_0_neigh_iterator(const Container_0 *C0, uint64_t x) {
            _C0 = C0;
            if (_C0->adj_contains(x) && !_C0->elements.empty()) {
                k = _C0->adj_map.find(x)->second;
                _ptr = _C0->elements_nodes[k].y();
            } else {
                _ptr = -1;
            }
        }

        value_type operator*() {
            return _ptr;
        }

        Container_0_neigh_iterator &operator++(int) {
            if (_ptr != -1)
                operator++();
            return *this;
        }

        Container_0_neigh_iterator &operator++() {
            if (_C0->elements[k].has_next()) {
                k = _C0->elements[k].next();
                _ptr = _C0->elements_nodes[k].y();
            } else {
                end();
            }
            return *this;
        }

        Container_0_neigh_iterator end() {
            _ptr = -1;
            return *this;
        }

        bool operator==(const Container_0_neigh_iterator &other) const {
            return _ptr == other._ptr;
        }

        bool operator!=(const Container_0_neigh_iterator &rhs) const {
            return !(*this == rhs);
        }

    };

}

#endif //DYNAMIC_K2TREE_CONTAINER_0_NEIGH_ITERATOR_HPP
