#ifndef __D_K_ITERSIZEATORSIZE_EDGE__
#define __D_K_ITERSIZEATORSIZE_EDGE__


#include <iterator>
#include <vector>
#include <array>
#include <memory>
#include <sdsl/k2_tree.hpp>

#include "Container0.hpp"
#include "../graph/Graph.hpp"
#include "utils.hpp"

using namespace std;
using namespace sdsl;
using namespace k2_tree_ns;

namespace dynamic_ktree {

    class DKtreeEdge {
    private:
        etype _x, _y;
    public:
        DKtreeEdge() {}
        DKtreeEdge(tuple<etype, etype> t) : _x(std::get<0>(t)), _y(std::get<1>(t)) {}
        DKtreeEdge(Edge e) : _x(e.x()), _y(e.y()) {}
        DKtreeEdge(etype x, etype y) : _x(x), _y(y) {}

        etype x() const {
            return _x;
        }
        etype y() const {
            return _y;
        }

        bool operator==(const DKtreeEdge& rhs) const {
            return _x == rhs._x && _y == rhs._y;
        }
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
            _tree = tree;
            _c0 = tree->first_container();
            _kcollection = _tree->k_collections();

            if (tree->get_number_nodes() == 0) {
                _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
                distance_C0 = -1;
                return;
            }
            for (unsigned int i = 0; i < _kcollection.size(); ++i)
                if (_kcollection[i] != nullptr)
                    _data_k_collection_curr[i] = _kcollection[i]->edge_begin();

            if (_c0.size() > 0) {
                distance_C0 = 0;
                _ptr = _convert_C0_edge(_c0.edge_begin());
            } else {
                distance_C0 = _c0.size();
                bool initialized = false;
                for (; _i < RS; ++_i) {
                    if (_kcollection[_i] != nullptr) {
                        _ptr = _convert_k_collection_edge(_data_k_collection_curr[_i]);
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
            for (int i = 0; i < _kcollection.size(); ++i) {
                cout << "ktree " << i << ": ";
                if (_kcollection[i] != nullptr)
                    cout << _kcollection[i]->get_number_edges() << endl;
                else
                    cout << "0 " << endl;

            }
            cout << endl;
        }

        virtual DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++() {
            if (distance_C0 < (int) _c0.size()) {
                distance_C0++;
                if (distance_C0 < (int) _c0.size()) {
                    _ptr = _convert_C0_edge(next(_c0.edge_begin(), distance_C0));
                    return *this;
                }
                else
                    for (int i = 0; i < RS; i++) {
                        if (_kcollection[i] != nullptr) {
                            assert(_data_k_collection_curr[i] == _kcollection[i]->edge_begin());
                            _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                            _i = i;
                            return *this;
                        }
                    }
            } else
                for (; _i < RS; _i++) {
                    if (_kcollection[_i] != nullptr) {
                        ++_data_k_collection_curr[_i];
                        if (_data_k_collection_curr[_i] == _kcollection[_i]->edge_end()) {
                            for (_i++; _i < RS; _i++) {
                                if (_kcollection[_i] != nullptr) {
                                    _ptr = _convert_k_collection_edge(_data_k_collection_curr[_i]);
                                    return *this;
                                }
                            }
                            break;
                        }
                        _ptr = _convert_k_collection_edge(_data_k_collection_curr[_i]);
                        return *this;
                    }
                }


            _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
            return *this;
        }

        virtual DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> &operator++(int) {
            if (!is_end())
                operator++(); // pre-increment
            return *this;
        }

        DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> end() {
            DKtreeEdgeIterator<dk_tree, k2tree, k2tree_edge_iterator> tmp;
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
                this->_kcollection = other._kcollection;
                this->_c0 = other._c0;
            }
            return *this;
        }

    private:
        bool is_end() {
            return _ptr.x() == MAX_SIZE_EDGE && _ptr.y() == MAX_SIZE_EDGE;
//            int containers = 0;
//            int end_containers = 0;
//            for (int j = 0; j < RS; ++j) {
//                if (_kcollection[j] != nullptr) {
//                    containers++;
//                    if ((_data_k_collection_curr)[j] == _kcollection[j]->edge_end()) {
//                        end_containers++;
//                    }
//                }
//            }
//            if (containers == 0) {
//                return _data_C0_curr == _c0.edge_end();
//            }
//            return containers == end_containers;
        }

        value_type _convert_C0_edge(vector<Edge>::const_iterator it) {
            return DKtreeEdge(*it);
        }

        value_type _convert_k_collection_edge(k2tree_edge_iterator &it) {
            return DKtreeEdge(*it);
        }

        //container
        const dk_tree *_tree;
        Container0 _c0;
        array<shared_ptr<k2tree>, RS> _kcollection;
        const idx_type MAX_SIZE_EDGE = 9999999;

        //state
        value_type _ptr = DKtreeEdge(MAX_SIZE_EDGE, MAX_SIZE_EDGE);
        vector<Edge>::const_iterator _data_C0_curr;
        int distance_C0 = -1;
        array<k2tree_edge_iterator, RS> _data_k_collection_curr;
        int _i = 0;
    };
}

#endif //_D_K_ITERSIZEATORSIZE_EDGE_
