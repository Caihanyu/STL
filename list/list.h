#include <iostream>

namespace cai {
    template<class T>
    struct list_node {
        T data;
        list_node<T>* prev;
        list_node<T>* next;

        list_node(const T& value = T())
            : data(value)
            , prev(nullptr)
            , next(nullptr)
        {}
    };

    template<class T>
    struct __list_iterator {
        typedef list_node<T> Node;
        Node* _node; // 指向节点的指针

        __list_iterator(Node* n = nullptr)
            : _node(n)
        {}

        T& operator*() {
            return _node->data;
        }

        T* operator->() { // 建议增加 -> 运算符
            return &(_node->data);
        }

        __list_iterator<T>& operator++() {
            _node = _node->next;
            return *this;
        }

        __list_iterator<T> operator++(int) { // 后置++
            __list_iterator<T> tmp(*this);
            _node = _node->next;
            return tmp;
        }

        bool operator!=(const __list_iterator<T>& it) const {
            return _node != it._node;
        }

        bool operator==(const __list_iterator<T>& it) const {
            return _node == it._node;
        }
    };

    template<class T>
    class list {
        typedef list_node<T> Node;
    private:
        Node* head;
    public:
        typedef __list_iterator<T> iterator;

        list() {
            head = new Node(); // 申请头节点
            head->prev = head;
            head->next = head;
        }

        // 析构函数需要简单实现，防止内存泄漏
        ~list() {
            clear();
            delete head;
        }

        void clear() {
            iterator it = begin();
            while (it != end()) {
                it = erase(it);
            }
        }

        iterator begin() {
            return iterator(head->next);
        }

        iterator end() {
            return iterator(head);
        }

        void push_back(const T& value) {
            Node* newnode = new Node(value);
            Node* tail = head->prev;

            tail->next = newnode;
            newnode->prev = tail;
            newnode->next = head;
            head->prev = newnode;
        }

        iterator erase(iterator pos) {
            Node* cur = pos._node;
            Node* prev = cur->prev;
            Node* next = cur->next;

            prev->next = next;
            next->prev = prev;
            delete cur;
            return iterator(next);
        }
    };
}