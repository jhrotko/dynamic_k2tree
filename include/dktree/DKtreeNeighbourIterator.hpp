//
// Created by joana on 05/07/20.
//

#ifndef _DKTREE_NEIGHBOUR_ITERATOR_HPP_
#define _DKTREE_NEIGHBOUR_ITERATOR_HPP_

#include "../graph/Graph.hpp"
#include <memory>

using namespace std;

template<class dktree, class ktree, class k2_tree_neighbour_iterator, class Container_0, class Container_0_it>
class DKtreeNeighbourIterator
        : public GraphIterator<DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it>> {
    using value_type = int;
    using pointer = shared_ptr<int>;
    using reference = int &;
    using iterator_category = forward_iterator_tag;
public:
    DKtreeNeighbourIterator() = default;

    DKtreeNeighbourIterator(const dktree *tree, Container_0 *C0, etype node) : tree(tree), node(node) {
        _c0_index = 0;
        _c0_neigh_it = C0->neighbour_begin(node);
        _c0_neigh_it_end = C0->neighbour_end();

        if (_c0_neigh_it != _c0_neigh_it_end) {
            _ptr = *_c0_neigh_it;
            return;
        } else {
            _c0_index = -1;
            for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                if (tree->k_collection[_curr_ktree] != nullptr) {
                    _k_collection_it = tree->k_collection[_curr_ktree]->neighbour_begin(node);
                    if (_k_collection_it != _k_collection_it_end) {
                        _ptr = *(_k_collection_it);
                        return;
                    }
                }
            }
        }
        end();
    }

    value_type operator*() {
        return _ptr;
    }

    virtual bool operator==(
            const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it> &rhs) const {
        return this->_ptr == rhs._ptr;
    }

    virtual bool operator!=(
            const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it> &rhs) const {
        return !(*this == rhs);
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it> &
    operator++(int) {
        if (_ptr != -1)
            operator++();
        return *this;
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it> &
    operator++() {
        if (_c0_index != -1) {
            _c0_neigh_it++;
            if (_c0_neigh_it != _c0_neigh_it_end)
                _ptr = *_c0_neigh_it;
            else {
                _c0_index = -1;
                return operator++();
            }
        } else {
            if (_curr_ktree == -1) {
                for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                    if (tree->k_collection[_curr_ktree] != nullptr) {
                        _k_collection_it = tree->k_collection[_curr_ktree]->neighbour_begin(node);
                        if (_k_collection_it != _k_collection_it_end) {
                            _ptr = *(_k_collection_it);
                            return *this;
                        }
                    }
                }
                end();
                return *this;
            } else {
                ++_k_collection_it;

                if (_k_collection_it == _k_collection_it_end) {
                    ++_curr_ktree;
                    if (_curr_ktree > tree->get_max_r()) {
                        end();
                        return *this;
                    }
                    for (; _curr_ktree <= tree->get_max_r(); ++_curr_ktree) {
                        if (tree->k_collection[_curr_ktree] != nullptr) {
                            _k_collection_it = tree->k_collection[_curr_ktree]->neighbour_begin(node);
                            if (_k_collection_it != _k_collection_it_end) {
                                _ptr = *(_k_collection_it);
                                return *this;
                            }
                        }
                    }
                    if (_curr_ktree > tree->get_max_r()) {
                        end();
                        return *this;
                    }

                } else
                    _ptr = *(_k_collection_it);
            }
        }
        return *this;
    }

    DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator, Container_0, Container_0_it> &end() {
        _ptr = -1;
        _c0_index = -1;
        return *this;
    }

private:
    const dktree *tree;
    etype node;
    value_type _ptr = -1;
    k2_tree_neighbour_iterator _k_collection_it, _k_collection_it_end = k2_tree_neighbour_iterator().end();

    Container_0_it _c0_neigh_it, _c0_neigh_it_end;
    int _c0_index = -1;
    int _curr_ktree = -1;
};

#endif //_DKTREE_NEIGHBOUR_ITERATOR_HPP_
