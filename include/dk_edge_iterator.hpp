#ifndef __D_K_ITERATOR_EDGE__
#define __D_K_ITERATOR_EDGE__

#define R 8

#include <iterator>
#include <vector>
#include <array>
#include <sdsl/k2_tree.hpp>
#include "edge.hpp"
#include "ktree_extended.hpp"

using namespace std;

namespace dynamic_ktree {

    class dktree_edge {
    public:
        idx_type x, y;

        dktree_edge(tuple<idx_type, idx_type> t) {
            x = std::get<0>(t);
            y = std::get<1>(t);
        }

        dktree_edge(edge_node e) : x(e.x), y(e.y) {}

        dktree_edge(idx_type x, idx_type y) : x(x), y(y) {}

        bool operator==(const dktree_edge &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const dktree_edge &rhs) const {
            return !(*this == rhs);
        }

        friend ostream &operator<<(ostream &os, const dktree_edge &edge) {
            os << edge.x << "  " << edge.y;
            return os;
        }
    };

    template<uint8_t k = 2,
            typename t_bv = bit_vector,
            typename t_rank = typename t_bv::rank_1_type,
            typename l_rank = typename t_bv::rank_1_type>
    class dk_edge_iterator {
        using k_tree = ktree_extended<k, t_bv, t_rank, l_rank>;
    public:
        using value_type = dktree_edge;
        using pointer = dktree_edge *;
        using reference = dktree_edge &;
        using iterator_category = forward_iterator_tag;

        dk_edge_iterator() = default;

        dk_edge_iterator(const vector<edge_node> &C0, const array<shared_ptr<k_tree>, R> &k_tree_array) :
                _data_C0(&C0), _data_k_collection(&k_tree_array) {
            _data_C0_curr = _data_C0->begin();
            _ptr = _convert_C0_edge(_data_C0_curr);
            for (int i = 0; i < _data_k_collection->size(); ++i)
                if ((*_data_k_collection)[i] != nullptr)
                    _data_k_collection_curr[i] = (*_data_k_collection)[i]->edge_begin();

        }

        dk_edge_iterator(dk_edge_iterator<k, t_bv, t_rank, l_rank> &it) {
            *this = it;
        }

        value_type operator*() {
            return *_ptr;
        }

        void print() {
            cout << "_data_CO" << endl;
            for (auto a: *_data_C0)
                cout << a << " ";
            cout << "_data_k_collection" << endl;
            for (int i = 0; i < _data_k_collection->size(); ++i) {
                if ((*_data_k_collection)[i] != nullptr)
                    cout << (*_data_k_collection)[i]->get_number_edges() << " ";
                else
                    cout << "0 ";
            }
            cout << endl;
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &operator=(const dk_edge_iterator<k, t_bv, t_rank, l_rank> &other) {
            if (this != &other) {
                this->_data_C0 = other._data_C0;
                this->_data_k_collection = other._data_k_collection;

                this->_ptr = other._ptr;
                this->_data_k_collection_curr = other._data_k_collection_curr;
                copy(other._data_k_collection_curr.begin(), other._data_k_collection_curr.end(),
                     this->_data_k_collection_curr.begin());
                this->distance_C0 = other.distance_C0; //FIXME:
            }
            return *this;
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &operator++() {
            if (distance_C0 <= _data_C0->size()) {
                distance_C0++;
                if (distance_C0 < _data_C0->size())
                    _ptr = _convert_C0_edge(next(_data_C0->begin(), distance_C0));
                else
                    for (int i = 0; i < R; i++) {
                        if ((*_data_k_collection)[i] != nullptr)
                            _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                    }
            } else
                increment();
            return *this;
        }

        void increment() {
            for (int i = 0; i < R; i++) {
                if ((*_data_k_collection)[i] != nullptr &&
                    _data_k_collection_curr[i] != (*_data_k_collection)[i]->edge_end()) {
                    _data_k_collection_curr[i]++;
                    _ptr = _convert_k_collection_edge(_data_k_collection_curr[i]);
                }
            }
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &operator++(int) {
            dk_edge_iterator<k, t_bv, t_rank, l_rank> *tmp = new dk_edge_iterator<k, t_bv, t_rank, l_rank>(
                    *this); // copy
            operator++(); // pre-increment
            return *tmp;
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> &end() {
            dk_edge_iterator<k, t_bv, t_rank, l_rank> *tmp;
            size_t j = -1;
            for (uint i = 0; i < R; ++i)
                if ((*_data_k_collection)[i] != nullptr)
                    j = i;
            if (j != -1)
                tmp = _convert_k_collection_edge_iterator((*_data_k_collection)[j]->edge_end(), j);
            else
                tmp = _convert_C0_iterator(_data_C0->end());
            return *tmp;
        }

        bool operator==(const dk_edge_iterator<k, t_bv, t_rank, l_rank> &rhs) const {
            return *(this->_ptr) == *(rhs._ptr);
        }

        bool operator!=(const dk_edge_iterator<k, t_bv, t_rank, l_rank> &rhs) const {
            return !(*this == rhs);
        }

    private:
        pointer _convert_C0_edge(vector<edge_node>::const_iterator it) {
            return new dktree_edge(it->x, it->y);
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> *_convert_C0_iterator(vector<edge_node>::const_iterator it) {
            dk_edge_iterator<k, t_bv, t_rank, l_rank> *new_it = new dk_edge_iterator<k, t_bv, t_rank, l_rank>(*this);
            new_it->_ptr = _convert_C0_edge(it);
            new_it->_data_C0_curr = it;
            return new_it;
        }

        pointer _convert_k_collection_edge(edge_iterator<k, t_bv, t_rank> &it) {
            return new dktree_edge(*it);
        }

        dk_edge_iterator<k, t_bv, t_rank, l_rank> *
        _convert_k_collection_edge_iterator(edge_iterator<k, t_bv, t_rank> it, uint i) {
            dk_edge_iterator<k, t_bv, t_rank, l_rank> *new_it = new dk_edge_iterator<k, t_bv, t_rank, l_rank>(*this);
            new_it->_ptr = _convert_k_collection_edge(it);
            new_it->_data_k_collection_curr[i] = it;
            return new_it;
        }

        //container
        const vector<edge_node> *_data_C0;
        const array<shared_ptr<k_tree>, R> *_data_k_collection;

        //state
        pointer _ptr;
        vector<edge_node>::const_iterator _data_C0_curr;
        uint distance_C0 = 0;
        array<edge_iterator<k, t_bv, t_rank>, R> _data_k_collection_curr;
    };
}

#endif //_D_K_ITERATOR_EDGE_
