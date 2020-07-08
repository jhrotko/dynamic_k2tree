//
// Created by joana on 05/07/20.
//

#ifndef _DKTREE_NEIGHBOUR_ITERATOR_HPP_
#define _DKTREE_NEIGHBOUR_ITERATOR_HPP_

#include "../graph/Graph.hpp"
#include <memory>

using namespace std;

template<class dktree, class ktree, class k2_tree_neighbour_iterator>
class DKtreeNeighbourIterator
        : public GraphIterator<DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator>> {
    using value_type = etype;
    using pointer = shared_ptr<etype>;
    using reference = etype &;
    using iterator_category = forward_iterator_tag;
public:
    DKtreeNeighbourIterator() {}

    DKtreeNeighbourIterator(const dktree *tree, etype node) : tree(tree), node(node) {
        tree->first_container().list_neighbours(node, C0_neighbours);

        if (C0_neighbours.size() != 0)
            _ptr = make_shared<value_type>(C0_neighbours[_c0_index]);
        else {
            _c0_index = -1;
            for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];
                if (curr_ktree != nullptr &&
                    curr_ktree->neighbour_begin(node) != curr_ktree->neighbour_end()) {
                    _ptr = make_shared<value_type>(*curr_ktree->neighbour_begin(node));
                    _curr_neigh_it = curr_ktree->neighbour_begin(node);
                    break;
                }
            }
        }
    }

    value_type operator*() {
        return *_ptr;
    }

    virtual bool operator==(const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &rhs) const {
        if (this->_ptr != NULL && rhs._ptr != NULL)
            return *(this->_ptr) == *(rhs._ptr);
        return false;
    }

    virtual bool operator!=(const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &rhs) const {
        return !(*this == rhs);
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &operator++(int) {
        operator++(); // pre-increment
        return *this;
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &operator++() {
        if (_c0_index != -1) {
            _c0_index++;
            if (_c0_index < C0_neighbours.size())
                *_ptr = C0_neighbours[_c0_index];
            else
                _c0_index = -1;
        } else {
            if (_curr_ktree == -1) {
                for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                    shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];
                    if (curr_ktree != nullptr &&
                        curr_ktree->neighbour_begin(node) != curr_ktree->neighbour_end()) {
                        _curr_neigh_it = curr_ktree->neighbour_begin(node);
                        *_ptr = *_curr_neigh_it;
                    }
                }
            } else {
                _curr_neigh_it++;

                auto curr_end = tree->k_collections()[_curr_ktree]->neighbour_end();
                if (_curr_neigh_it == curr_end) {
                    if (_curr_ktree < tree->get_max_r()) {
                        *_ptr = -1;
                        return *this;
                    } else {
                        _curr_ktree++;
                        for (; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                            shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];
                            if (curr_ktree != nullptr &&
                                curr_ktree->neighbour_begin(node) != curr_ktree->neighbour_end()) {
                                _curr_neigh_it = curr_ktree->neighbour_begin(node);
                            }
                        }
                    }
                } else
                    *_ptr = *_curr_neigh_it;
            }
            return *this;
        }
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> end() {
        DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> tmp(*this);
        tmp._ptr = make_shared<value_type>(-1);
//        tmp._c0_index = -1;
//        tmp._curr_ktree = tree->get_max_r() + 1;
        return tmp;
    }

private:
    const dktree *tree;
    etype node;
    pointer _ptr;

    //FIXME: Create neighbour iterator in C0
    vector<etype> C0_neighbours;
    int _c0_index = 0;
    int _curr_ktree = -1;
    k2_tree_neighbour_iterator _curr_neigh_it;
};

#endif //_DKTREE_NEIGHBOUR_ITERATOR_HPP_
