#ifndef __D_K_ITERATOR_VERTICE__
#define __D_K_ITERATOR_VERTICE__

#include <iterator>
#include <memory>
#include "../graph/Graph.hpp"

using namespace std;

namespace dynamic_ktree {

    template<class dktree>
    class DKtreeNodeIterator : public GraphNodeIterator<DKtreeNodeIterator<dktree>> {
    public:
        using value_type = int;
        using pointer = shared_ptr<int>;
        using reference = int &;
        using iterator_category = forward_iterator_tag;

        DKtreeNodeIterator() {
            _ptr = make_shared<int>(-1);
            end();
        }

        DKtreeNodeIterator(const dktree *tree) {
            tree_size = tree->get_number_nodes();
            if (tree_size > 0) {
                _ptr = make_shared<int>(0);
            } else {
                _ptr = make_shared<int>(-1);
            }
        }

        DKtreeNodeIterator(DKtreeNodeIterator<dktree> &other) {
            this->_ptr = other._ptr;
            this->tree_size = other.tree_size;
        }

        value_type operator*() const {
            return *_ptr;
        }

        DKtreeNodeIterator<dktree> end() {
            DKtreeNodeIterator<dktree> tmp(*this);
            tmp._ptr = make_shared<value_type>(-1);
            return tmp;
        }

        virtual bool operator==(const DKtreeNodeIterator<dktree> &rhs) const {
            if (this->_ptr != NULL && rhs._ptr != NULL)
                return *(this->_ptr) == *(rhs._ptr);
            return false;
        }

        virtual bool operator!=(const DKtreeNodeIterator<dktree> &rhs) const {
            return !(*this == rhs);
        }

        virtual DKtreeNodeIterator<dktree> &operator++() {
            if (this->_ptr != NULL) {
                if (*(this->_ptr) >= tree_size - 1 || *(this->_ptr) == -1)
                    *_ptr = -1;
                else
                    *_ptr += 1;
            }
        }

        virtual DKtreeNodeIterator<dktree> &operator++(int) {
            operator++(); // pre-increment
            return *this;
        }

        virtual DKtreeNodeIterator<dktree> &operator=(const DKtreeNodeIterator<dktree> &other) {
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
