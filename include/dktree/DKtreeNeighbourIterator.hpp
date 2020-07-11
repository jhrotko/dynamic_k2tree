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
    using value_type = int;
    using pointer = shared_ptr<int>;
    using reference = int &;
    using iterator_category = forward_iterator_tag;
public:
    DKtreeNeighbourIterator() {
        end();
    }

    DKtreeNeighbourIterator(const dktree *tree, etype node) : tree(tree), node(node) {
        tree->first_container().list_neighbours(node, C0_neighbours);

        if (C0_neighbours.size() > 0) {
            _ptr = C0_neighbours[_c0_index];
            return;
        }
        else {
            _c0_index = -1;
            for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];

                if (curr_ktree != nullptr) {
                    k2_tree_neighbour_iterator neigh_it = curr_ktree->neighbour_begin(node);
                    k2_tree_neighbour_iterator neigh_it_end = curr_ktree->neighbour_end();
                    if (*neigh_it != *neigh_it_end) {
                        _ptr = *neigh_it;
                        _curr_neigh_it = neigh_it;
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

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &operator=(
            const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &other) {
        if (this != &other) {
            tree = other.tree;
            node = other.node;
            _ptr = other._ptr;

            C0_neighbours = other.C0_neighbours;
            _c0_index = other._c0_index;
            _curr_ktree =  other._curr_ktree;
            _curr_neigh_it = other._curr_neigh_it;
        }
        return *this;
    }

    virtual bool operator==(const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &rhs) const {
        return this->_ptr == rhs._ptr;
    }

    virtual bool operator!=(const DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &rhs) const {
        return !(*this == rhs);
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &operator++(int) {
        if(_ptr != -1)
            operator++(); // pre-increment
        return *this;
    }

    virtual DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> &operator++() {
        if (_c0_index != -1) {
            _c0_index++;
            if (_c0_index < C0_neighbours.size())
                _ptr = C0_neighbours[_c0_index];
            else
                _c0_index = -1;
        } else {
            if (_curr_ktree == -1) {
                for (_curr_ktree = 0; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                    shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];
                    if (curr_ktree != nullptr &&
                        curr_ktree->neighbour_begin(node) != curr_ktree->neighbour_end()) {
                        _curr_neigh_it = curr_ktree->neighbour_begin(node);
                        _ptr = *_curr_neigh_it;
                    }
                }
            } else {
                _curr_neigh_it++;

                if (_curr_neigh_it == tree->k_collections()[_curr_ktree]->neighbour_end()) {
                    _curr_ktree++;
                    for (; _curr_ktree <= tree->get_max_r(); _curr_ktree++) {
                        shared_ptr<ktree> curr_ktree = tree->k_collections()[_curr_ktree];
                        if (curr_ktree != nullptr &&
                            curr_ktree->neighbour_begin(node) != curr_ktree->neighbour_end()) {
                            _curr_neigh_it = curr_ktree->neighbour_begin(node);
                        }
                    }
                    if (_curr_ktree > tree->get_max_r()) {
                        end();
                        return *this;
                    }

                } else
                    _ptr = *_curr_neigh_it;
            }
            return *this;
        }
    }

    DKtreeNeighbourIterator<dktree, ktree, k2_tree_neighbour_iterator> end() {
        _ptr = -1;
        _c0_index = -1;
        return *this;
    }

private:
    const dktree *tree;
    etype node;
    value_type _ptr;

    //FIXME: Create neighbour iterator in C0
    vector<etype> C0_neighbours;
    int _c0_index = 0;
    int _curr_ktree = -1;
    k2_tree_neighbour_iterator _curr_neigh_it;
};

#endif //_DKTREE_NEIGHBOUR_ITERATOR_HPP_
