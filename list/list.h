#include <iostream>
#include <assert.h>

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

    // 迭代器类，模版参数对应类型、引用类型和指针类型
    // typedef __list_iterator<T, T&, T*> iterator;
    // typedef __list_iterator<T, const T&, const T*> const_iterator;

    template<class T, class Ref, class Ptr>
    struct __list_iterator {
        typedef __list_iterator<T, Ref, Ptr> self;
        typedef list_node<T> Node;
        Node* _node; // 指向节点的指针

        __list_iterator(Node* n = nullptr)
            : _node(n)
        {}

        Ref operator*() {
            return _node->data;
        }

        Ptr operator->() { // 建议增加 -> 运算符
            return &(_node->data);
        }

        self& operator++() {
            _node = _node->next;
            return *this;
        }

        self operator++(int) { // 后置++
            self tmp(*this);
            _node = _node->next;
            return tmp;
        }

        self operator--(int) { // 后置--
            self tmp(*this);
            _node = _node->prev;
            return tmp;
        }

        self& operator--() {
            _node = _node->prev;
            return *this;
        }

        bool operator!=(const self& it) const {
            return _node != it._node;
        }

        bool operator==(const self& it) const {
            return _node == it._node;
        }
    };

    template<class T>
    class list {
        typedef list_node<T> Node;
    private:
        Node* head;
    public:
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, const T&, const T*> const_iterator;

        void empty_init()
        {
            head = new Node(); // 申请头节点
            head->prev = head;
            head->next = head;
        }

        list() 
        {
            empty_init();
        }

        list(const list<T>& other)
        {
            empty_init();

            for(auto& value : other) {
                push_back(value);
            }
        }

        void swap(list<T>& other) {
            std::swap(head, other.head);
        }

        list<T>& operator=(const list<T>& other)
        {
            if (this != &other) {
                list<T> tmp(other); // 复制构造
                swap(tmp); // 交换资源
            }
            return *this;
        }

        // 析构函数需要简单实现，防止内存泄漏
        ~list() 
        {
            clear();
            delete head;
        }

        void clear() 
        {
            iterator it = begin();
            while (it != end()) {
                it = erase(it);
            }
        }

        iterator begin() 
        {
            // 可以通过单参数构造隐式类型转换来简化代码，这里显式写出来说明
            return iterator(head->next);
        }

        iterator end() 
        {
            return iterator(head);
        }

        const_iterator begin() const
        {
            // 可以通过单参数构造隐式类型转换来简化代码，这里显式写出来说明
            return const_iterator(head->next);
        }

        const_iterator end() const
        {
            return const_iterator(head);
        }

        size_t size() const {
            size_t count = 0;
            Node* cur = head->next;
            while (cur != head) {
                ++count;
                cur = cur->next;
            }
            return count;
        }

        // 增删查改
        iterator insert(iterator pos, const T& value)
        {
            Node* newnode = new Node(value);
            Node* prev = pos._node->prev;
            newnode->next = pos._node;
            newnode->prev = prev;
            prev->next = newnode;
            pos._node->prev = newnode;

            return iterator(newnode);
        }

        void push_front(const T& value) {
            insert(begin(), value);
        }

        void push_back(const T& value) {
            insert(end(), value);
        }

        iterator erase(iterator pos) {
            assert(pos != end()); // 不能删除end()位置的元素

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