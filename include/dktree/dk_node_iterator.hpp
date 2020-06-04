#ifndef __D_K_ITERATOR_VERTICE__
#define __D_K_ITERATOR_VERTICE__

#include <iterator>

using namespace std;

namespace dynamic_ktree {

    template<class dktree>
    class dk_node_iterator {
    public:
        using value_type = int;
        using pointer = int *;
        using reference = int &;
        using iterator_category = forward_iterator_tag;

        dk_node_iterator() {
            _ptr = (int *) malloc(sizeof(int));
            end();
        }

        dk_node_iterator(const dktree *tree) {
            tree_size = tree->get_number_nodes();
            _ptr = (int *) malloc(sizeof(int));
            if (tree_size > 0) {
                *_ptr = 0;
            } else {
                end();
            }
        }

        dk_node_iterator(dk_node_iterator<dktree> &other) {
            this->_ptr = other._ptr;
            this->tree_size = other.tree_size;
        }

        value_type operator*() const {
            return *_ptr;
        }

        dk_node_iterator<dktree> &end() {
            *_ptr = -1;
            return *this;
        }

        bool operator==(const dk_node_iterator<dktree> &rhs) const {
            if(this->_ptr != NULL && rhs._ptr != NULL)
                return *(this->_ptr) == *(rhs._ptr);
            return false;
        }

        bool operator!=(const dk_node_iterator<dktree> &rhs) const {
            return !(*this == rhs);
        }

        dk_node_iterator<dktree> &operator++() {
            if(this->_ptr != NULL)
                *_ptr = *_ptr + 1;
                if(*(this->_ptr) >= tree_size)
                    end();
        }

        dk_node_iterator<dktree> &operator++(int) {
            dk_node_iterator<dktree> *tmp = new dk_node_iterator<dktree>(*this); // copy
            operator++(); // pre-increment
            return *tmp;
        }

        dk_node_iterator<dktree> &operator=(const dk_node_iterator<dktree> &other) {
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
