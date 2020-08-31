#ifndef __D_K_ITERATOR_VERTICE__
#define __D_K_ITERATOR_VERTICE__

#include <iterator>
#include <memory>
#include <array>
#include <vector>
#include "../graph/Graph.hpp"

using namespace std;

namespace dynamic_ktree {

    template<class dktree, class k_tree>
    class DKtreeNodeIterator : public GraphIterator<DKtreeNodeIterator<dktree, k_tree>> {
    public:
        using value_type = int;
        using pointer = shared_ptr<int>;
        using reference = int &;
        using iterator_category = forward_iterator_tag;

        DKtreeNodeIterator() {
            _ptr = -1;
            end();
        }

        DKtreeNodeIterator(const dktree *tree) {
            this->tree = tree;
            tree_size = tree->get_number_nodes();
            nodes.resize(tree_size);
            nodes.assign(nodes.size(), false);
            if (tree_size > 0) {
                node_it_container = tree->first_container().node_begin();
                if(node_it_container != tree->first_container().node_end()) {
                    _ptr = node_it_container->first;
                    nodes[_ptr] = true;
                    return;
                }
                else {
                    for(; k_i < R; k_i++) {
                        if(tree->k_collections()[k_i] != nullptr &&
                        tree->k_collections()[k_i]->node_begin() != tree->k_collections()[k_i]->node_end()) {
                            node_it_collections[k_i] = tree->k_collections()[k_i]->node_begin();
                            _ptr = *node_it_collections[k_i];
                            nodes[_ptr] = true;
                            return;
                        }
                    }
                }
            }
            _ptr = -1;
        }

        DKtreeNodeIterator(DKtreeNodeIterator<dktree, k_tree> &other) {
            tree = other.tree;
            _ptr = other._ptr;
            tree_size = other.tree_size;
            node_it_container = other.node_it_container;
            node_it_collections = other.node_it_collections;
            k_i = other.k_i;
            nodes = other.nodes;
        }

        value_type operator*() const {
            return _ptr;
        }

        DKtreeNodeIterator<dktree, k_tree> end() {
            DKtreeNodeIterator<dktree, k_tree> tmp(*this);
            tmp._ptr = -1;
            return tmp;
        }

        virtual bool operator==(const DKtreeNodeIterator<dktree, k_tree> &rhs) const {
            return _ptr == rhs._ptr;
        }

        virtual bool operator!=(const DKtreeNodeIterator<dktree, k_tree> &rhs) const {
            return !(*this == rhs);
        }

        virtual DKtreeNodeIterator<dktree, k_tree> &operator++() {
            if(_ptr != -1) {
                if (node_it_container != tree->first_container().node_end()) {
                    node_it_container++;
                    if(node_it_container != tree->first_container().node_end() &&
                    !nodes[node_it_container->first]) {
                        _ptr = node_it_container->first;
                        nodes[_ptr] = true;
                        return *this;
                    } else {
                        return operator++();
                    }
                } else if(k_i < R) {
                    if(tree->k_collections()[k_i] != nullptr && next_collection(k_i)) {
                        return *this;
                    } else {
                        for(; k_i < R; k_i++) {
                            if(tree->k_collections()[k_i] != nullptr &&
                            tree->k_collections()[k_i]->node_begin() != tree->k_collections()[k_i]->node_end()) {
                                node_it_collections[k_i] = tree->k_collections()[k_i]->node_begin();

                                if(!nodes[*node_it_collections[k_i]]) {
                                    _ptr = *node_it_collections[k_i];
                                    nodes[_ptr] = true;
                                    return *this;
                                } else {
                                    if(next_collection(k_i))
                                        return *this;
                                }
                            }
                        }
                        if(k_i == R)
                            _ptr = -1;
                    }
                }
            }
            return *this;
        }

        virtual DKtreeNodeIterator<dktree, k_tree> &operator++(int) {
            operator++(); // pre-increment
            return *this;
        }

        virtual DKtreeNodeIterator<dktree, k_tree> &operator=(const DKtreeNodeIterator<dktree, k_tree> &other) {
            if (this != &other) {
                tree = other.tree;
                _ptr = other._ptr;
                tree_size = other.tree_size;
                node_it_container = other.node_it_container;
                node_it_collections = other.node_it_collections;
                k_i = other.k_i;
                nodes = other.nodes;
            }
            return *this;
        }

    private:
        bool next_collection(uint i) {
            node_it_collections[i]++;
            if(node_it_collections[i] != tree->k_collections()[i]->node_end()
                && !nodes[*node_it_collections[i]]) {
                _ptr = *node_it_collections[i];
                nodes[*node_it_collections[i]] = true;
                return true;
            } else if(node_it_collections[i] != tree->k_collections()[i]->node_end()
                      && nodes[*node_it_collections[i]]) {
                return next_collection(i);
            }
            return false;
        }
        //state
        const dktree* tree;
        value_type _ptr;
        int tree_size = 0;
        unordered_map<etype, etype>::const_iterator node_it_container;
        std::array<sdsl::node_iterator<k_tree>, R> node_it_collections;
        uint  k_i = 0;
        std::vector<bool> nodes;
    };
}

#endif
