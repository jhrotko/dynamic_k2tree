#ifndef __D_K_ITERATOR_EDGE__
#define __D_K_ITERATOR_EDGE__


#include <iterator>
#include <vector>
#include <array>
#include <memory>
#include <sdsl/k2_tree.hpp>

#include "Container_0.hpp"
#include "../graph/Graph.hpp"
#include "utils.hpp"

using namespace std;
using namespace sdsl;
using namespace k2_tree_ns;

namespace dynamic_ktree {

    #define R 8

    class DKtreeEdge : public Edge {
    public:
        DKtreeEdge() {}

        DKtreeEdge(tuple<idx_type, idx_type> t) : Edge(std::get<0>(t), std::get<1>(t)) {}

//        DKtreeEdge(NodeEdge e) : Edge(e.x(), e.y()) {}

        DKtreeEdge(idx_type x, idx_type y) : Edge(x, y) {}
    };

    template<class dk_tree, class k2tree, class k2tree_edge_iterator>
    class DKtreeEdgeIterator : public GraphEdgeIterator<DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator>> {
    public:
        using value_type = DKtreeEdge;
        using pointer = shared_ptr<DKtreeEdge>;
        using reference = DKtreeEdge &;
        using iterator_category = forward_iterator_tag;

        DKtreeEdgeIterator() {}

        DKtreeEdgeIterator(const dk_tree *tree) {
            //Initialize curr pointers
            this->_tree = tree;

            if (tree->get_number_nodes() == 0) {
                _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
                distance_C0 = -1;
                return;
            }
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
                    _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
                    distance_C0 = -1;
                }
            }
        }

        value_type operator*() {
            return _ptr;
        }

        virtual etype x() const {
            return _ptr.x();
        }

        virtual etype y() const {
            return _ptr.y();
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

        virtual DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++() {
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
                _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
            return *this;
        }

        virtual DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++(int) {
            if (*this != end())
                operator++(); // pre-increment
            return *this;
        }

        DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> end() {
            DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> tmp(*this);
            tmp._ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
            return tmp;
        }

        virtual bool operator==(const DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &rhs) const {
            return _ptr == rhs._ptr;
        }

        virtual bool operator!=(const DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &rhs) const {
            return !(*this == rhs);
        }

        virtual DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &
        operator=(const DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &other) {
            if (this != &other) {
                this->_tree = other._tree;

                this->_data_k_collection_curr = other._data_k_collection_curr;
                this->_ptr = other._ptr;
                this->distance_C0 = other.distance_C0;
            }
            return *this;
        }

    private:
        bool is_end() {
            if (_ptr == DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE))
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

        value_type _convert_C0_edge(vector<tuple<etype, etype>>::const_iterator it) {
            return DKtreeEdge(get<0>(*it), get<1>(*it));
        }

        value_type _convert_k_collection_edge(k2tree_edge_iterator &it) {
            return DKtreeEdge(*it);
        }

        //container
        const dk_tree *_tree;
        const idx_type MAX_SIZE_EDGE = 9999999;

        //state
        value_type _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
        vector<tuple<etype, etype>>::const_iterator _data_C0_curr;
        int distance_C0 = -1;
        array<k2tree_edge_iterator, R> _data_k_collection_curr;
    };
}

#endif //_D_K_ITERATOR_EDGE_
