#ifndef __D_K_ITERATOR_VERTICE__
#define __D_K_ITERATOR_VERTICE__

#include <iterator>
#include <memory>
#include "../graph/Graph.hpp"

using namespace std;

namespace dynamic_ktree {

    template<class dktree>
    class DKtreeNodeIterator : public GraphIterator<DKtreeNodeIterator<dktree>> {
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
            tree_size = tree->get_number_nodes();
            if (tree_size > 0) {
                _ptr = 0;
            } else {
                _ptr = -1;
            }
        }

        DKtreeNodeIterator(DKtreeNodeIterator<dktree> &other) {
            _ptr = other._ptr;
            tree_size = other.tree_size;
        }

        value_type operator*() const {
            return _ptr;
        }

        DKtreeNodeIterator<dktree> end() {
            DKtreeNodeIterator<dktree> tmp(*this);
            tmp._ptr = -1;
            return tmp;
        }

        virtual bool operator==(const DKtreeNodeIterator<dktree> &rhs) const {
            return _ptr == rhs._ptr;
        }

        virtual bool operator!=(const DKtreeNodeIterator<dktree> &rhs) const {
            return !(*this == rhs);
        }

        virtual DKtreeNodeIterator<dktree> &operator++() {
            if (_ptr >= tree_size - 1 || _ptr == -1)
                _ptr = -1;
            else
                _ptr += 1;
            return *this;
        }

        virtual DKtreeNodeIterator<dktree> &operator++(int) {
            operator++(); // pre-increment
            return *this;
        }

        virtual DKtreeNodeIterator<dktree> &operator=(const DKtreeNodeIterator<dktree> &other) {
            if (this != &other) {
                _ptr = other._ptr;
                tree_size = other.tree_size;
            }
            return *this;
        }

    private:
        //state
        value_type _ptr;
        int tree_size = 0;
    };
}

#endif
