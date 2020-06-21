#ifndef __D_K_ITERATOR_EDGE__
#define __D_K_ITERATOR_EDGE__

#define R 8

#include <iterator>
#include <vector>
#include <array>
#include <sdsl/k2_tree.hpp>

#include "Container_0.hpp"
#include "../graph/Graph.hpp"
#include "utils.hpp"

using namespace std;
using namespace sdsl;
using namespace k2_tree_ns;

namespace dynamic_ktree {

    class dktree_edge : public Edge {
    public:
        dktree_edge(tuple<idx_type, idx_type> t) : Edge(std::get<0>(t), std::get<1>(t)) {}
        dktree_edge(NodeDouble e) : Edge(e.x(), e.y()) {}
        dktree_edge(idx_type x, idx_type y) : Edge(x, y) {}
    };

    template<class dk_tree, class k2tree, class k2tree_edge_iterator>
    class dk_edge_iterator: virtual public GraphEdgeIterator<dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator>> {
    public:
        using value_type = dktree_edge;
        using pointer = dktree_edge *;
        using reference = dktree_edge &;
        using iterator_category = forward_iterator_tag;

        dk_edge_iterator() {
            _ptr = new dktree_edge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
        }

        dk_edge_iterator(const dk_tree *tree) {
            //Initialize curr pointers
            this->_tree = tree;

            for (int i = 0; i < _tree->k_collections().size(); ++i)
                if (_tree->k_collections()[i] != nullptr)
                    _data_k_collection_curr[i] = _tree->k_collections()[i]->edge_begin();

            if (_tree->first_container().size() > 0) {
                distance_C0 = 0;
                _ptr = _convert_C0_edge(_tree->first_container().edge_begin());
            } else {
                distance_C0 = _tree->first_container().size();
                bool initialized = false;
                for (int i = 0; i < R; ++i) {
                    if (_tree->k_collections()[i] != nullptr) {
                        _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                        initialized = true;
                        break;
                    }
                }
                if (!initialized) {
                    //begin == end
                    _ptr = new dktree_edge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
                    distance_C0 = -1;
                }
            }
        }

        dk_edge_iterator(dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &it) {
            *this = it;
        }

        value_type operator*() {
            return *_ptr;
        }

        virtual etype x() const {
            return _ptr->x();
        }

        virtual etype y() const {
            return _ptr->y();
        }

        void print() {
            cout << "_data_CO" << endl;
//            for (auto a: *_data_C0)
//                cout << a << " ";
            cout << "_data_k_collection " << endl;
            for (int i = 0; i < _tree->k_collections().size(); ++i) {
                cout << "ktree " << i << ": ";
                if (_tree->k_collections()[i] != nullptr)
                    cout << _tree->k_collections()[i]->get_number_edges() << endl;
                else
                    cout << "0 " << endl;

            }
            cout << endl;
        }


        dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &operator=(const dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &other) {
            if (this != &other) {
                this->_tree = other._tree;

                this->_data_k_collection_curr = other._data_k_collection_curr;
                this->_ptr = other._ptr;
                this->distance_C0 = other.distance_C0;
            }
            return *this;
        }

        dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++() {
            if (distance_C0 < _tree->first_container().size()) {
                distance_C0++;
                if (distance_C0 < _tree->first_container().size())
                    _ptr = _convert_C0_edge(next(_tree->first_container().edge_begin(), distance_C0));
                else
                    for (int i = 0; i < R; i++) {
                        if (_tree->k_collections()[i] != nullptr) {
                            assert(_data_k_collection_curr[i] == _tree->k_collections()[i]->edge_begin());
                            _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                            break;
                        }
                    }
            } else
                for (int i = 0; i < R; i++) {
                    if (_tree->k_collections()[i] != nullptr &&
                        _data_k_collection_curr[i] != _tree->k_collections()[i]->edge_end()) {
                        _data_k_collection_curr[i]++;
                        if (_data_k_collection_curr[i] == _tree->k_collections()[i]->edge_end()) {
                            for (int j = i + 1; j < R; j++) {
                                if (_tree->k_collections()[j] != nullptr &&
                                    _data_k_collection_curr[j] != _tree->k_collections()[j]->edge_end()) {
                                    _ptr = _convert_k_collection_edge(_data_k_collection_curr[j]);
                                    return *this;
                                }
                            }
                        }
                        _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                        break;
                    }
                }

            if (is_end())
                _ptr = new dktree_edge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
            return *this;
        }

        virtual dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++(int) {
            dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> *tmp = new dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator>(
                    *this); // copy
            if (*this != end())
                operator++(); // pre-increment
            return *tmp;
        }

        friend void
        swap(dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &rhs, dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &lhs) {
            if (lhs != rhs) {
                std::swap(lhs._tree, rhs._tree);
                std::swap(lhs._ptr, rhs._ptr);
                std::swap(lhs._data_C0_curr, rhs._data_C0_curr);
                std::swap(lhs.distance_C0, rhs.distance_C0);
                std::swap(lhs._data_k_collection_curr, rhs._data_k_collection_curr);
            }
        }

        dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &end() {
            dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> *tmp = new dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator>(
                    *this);
            tmp->_ptr = new dktree_edge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
            return *tmp;
        }

        virtual bool operator==(const dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &rhs) const {
            return *(this->_ptr) == *(rhs._ptr);
        }

        virtual bool operator!=(const dk_edge_iterator<dk_tree, k2tree, k2tree_edge_iterator> &rhs) const {
            return !(*this == rhs);
        }

    private:
        bool is_end() {
            if (*_ptr == dktree_edge(MAX_SIZE_EDGE, MAX_SIZE_EDGE))
                return true;
            int containers = 0;
            int end_containers = 0;
            for (int j = 0; j < R; ++j) {
                if (_tree->k_collections()[j] != nullptr) {
                    containers++;
                    if ((_data_k_collection_curr)[j] == _tree->k_collections()[j]->edge_end()) {
                        end_containers++;
                    }
                }
            }
            if (containers == 0) {
                return _data_C0_curr == _tree->first_container().edge_end();
            }
            return containers == end_containers;
        }

        pointer _convert_C0_edge(vector<NodeDouble>::const_iterator it) {
            return new dktree_edge(it->x(), it->y());
        }

        pointer _convert_k_collection_edge(k2tree_edge_iterator &it) {
            return new dktree_edge(*it);
        }

        //container
        const dk_tree *_tree;
        const idx_type  MAX_SIZE_EDGE = 9999999;

        //state
        pointer _ptr;
        vector<NodeDouble>::const_iterator _data_C0_curr;
        int distance_C0 = -1;
        array<k2tree_edge_iterator, R> _data_k_collection_curr;
    };
}

#endif //_D_K_ITERATOR_EDGE_
