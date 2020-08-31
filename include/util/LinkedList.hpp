#ifndef DYNAMIC_K2TREE_LINKEDLIST_HPP
#define DYNAMIC_K2TREE_LINKEDLIST_HPP

class LinkedNode;

class LinkedNode {
public:
    LinkedNode *_next, *_prev;
    unsigned long _item;

    LinkedNode() {}

    LinkedNode(unsigned long it) : _item(it) {
        _next = nullptr;
        _prev = nullptr;
    }

    LinkedNode(unsigned long it, LinkedNode *next, LinkedNode *prev) : _item(it), _next(next), _prev(prev) {}

    ~LinkedNode() {}
};

class LinkedList {
private:
    LinkedNode *_head = nullptr;
    LinkedNode *_tail = nullptr;
public:
    LinkedList() {}

    LinkedList(LinkedNode *head) : _head(head) {
        _head->_prev = nullptr;
        _head->_next = nullptr;
        _tail = _head;
    }

    ~LinkedList() {
        if (_head != nullptr) {
            if (_head->_next == nullptr) {
                delete _head;
                return;
            }

            while (_head->_next != nullptr) {
                LinkedNode *copy_next_head = _head->_next;
                delete _head;
                _head = copy_next_head;
            }
            delete _head;
        }
    }

    void push(unsigned long item) {
        if (_head == nullptr) {
            _head = new LinkedNode(item);
            _tail = _head;
            return;
        }
        if (_head != nullptr && _head->_next == nullptr) {
            LinkedNode *newNode = new LinkedNode(item, nullptr, _head);
            _head->_next = newNode;
            _tail = newNode;
            return;
        }
        LinkedNode *newNode = new LinkedNode(item, nullptr, _tail->_prev);
        _tail->_next = newNode;
        _tail = newNode;
    }

    unsigned long pop() {
        if (_head != nullptr) {
            if (_head->_next == nullptr) {
                unsigned long val = _head->_item;
                delete _head;
                _tail = nullptr;
                _head = nullptr;
                return val;
            }

            unsigned long val = _head->_item;
            _head->_next->_prev = nullptr;
            LinkedNode *newHead = _head->_next;
            delete _head;
            _head = newHead;

            return val;
        }
    }
};

#endif //DYNAMIC_K2TREE_LINKEDLIST_HPP
