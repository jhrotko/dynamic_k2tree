//
// Created by joana on 04/09/20.
//

#ifndef DYNAMIC_K2TREE_CONTAINER0NEIGHITERATOR_HPP
#define DYNAMIC_K2TREE_CONTAINER0NEIGHITERATOR_HPP

#include <iterator>

namespace dynamic_ktree {

    template<class Container_0>
    class Container0NeighIterator {
    private:
        const Container_0 *_C0;
        int64_t _ptr;
        size_t k;
    public:
        using value_type = int64_t;
        using pointer = int64_t*;
        using reference = int64_t &;
        using iterator_category = std::forward_iterator_tag;

        Container0NeighIterator() {}

        Container0NeighIterator(const Container_0 *C0, uint64_t x) {
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

        Container0NeighIterator &operator++(int) {
            if (_ptr != -1)
                operator++();
            return *this;
        }

        Container0NeighIterator &operator++() {
            if (_C0->elements[k].has_next()) {
                k = _C0->elements[k].next();
                _ptr = _C0->elements_nodes[k].y();
            } else {
                end();
            }
            return *this;
        }

        Container0NeighIterator end() {
            _ptr = -1;
            return *this;
        }

        bool operator==(const Container0NeighIterator &other) const {
            return _ptr == other._ptr;
        }

        bool operator!=(const Container0NeighIterator &rhs) const {
            return !(*this == rhs);
        }

    };

}

#endif //DYNAMIC_K2TREE_CONTAINER0NEIGHITERATOR_HPP
