#ifndef __D_K_ITERATOR_VERTICE__
#define __D_K_ITERATOR_VERTICE__

#include <iterator>

using namespace std;

namespace dynamic_ktree {

    template<class dktree>
    class DKtreeNodeIterator {
    public:
        using value_type = int;
        using pointer = int *;
        using reference = int &;
        using iterator_category = forward_iterator_tag;

        DKtreeNodeIterator() {
            _ptr = (int *) malloc(sizeof(int));
            end();
        }

        DKtreeNodeIterator(const dktree *tree) {
            tree_size = tree->get_number_nodes();
            _ptr = (int *) malloc(sizeof(int));
            if (tree_size > 0) {
                *_ptr = 0;
            } else {
                end();
            }
        }

        DKtreeNodeIterator(DKtreeNodeIterator<dktree> &other) {
            this->_ptr = other._ptr;
            this->tree_size = other.tree_size;
        }

        value_type operator*() const {
            return *_ptr;
        }

        DKtreeNodeIterator<dktree> &end() {
            *_ptr = -1;
            return *this;
        }

        bool operator==(const DKtreeNodeIterator<dktree> &rhs) const {
            if(this->_ptr != NULL && rhs._ptr != NULL)
                return *(this->_ptr) == *(rhs._ptr);
            return false;
        }

        bool operator!=(const DKtreeNodeIterator<dktree> &rhs) const {
            return !(*this == rhs);
        }

        DKtreeNodeIterator<dktree> &operator++() {
            if(this->_ptr != NULL)
                *_ptr = *_ptr + 1;
                if(*(this->_ptr) >= tree_size)
                    end();
        }

        DKtreeNodeIterator<dktree> &operator++(int) {
            DKtreeNodeIterator<dktree> *tmp = new DKtreeNodeIterator<dktree>(*this); // copy
            operator++(); // pre-increment
            return *tmp;
        }

        DKtreeNodeIterator<dktree> &operator=(const DKtreeNodeIterator<dktree> &other) {
            if (this != &other) {
                this->_ptr = other._ptr;
                this->tree_size = other.tree_size;
            }
            return *this;
        }

    private:
        //state
        pointer _ptr;
        size_t tree_size = 0;
    };
}

#endif
