#pragma once
#include <cstddef>
#include <iostream>
#include <string>

enum Color
{
    RED,
    BLACK
};

template <typename T>
struct RBtreeNode
{
    RBtreeNode<T>* _left;
    RBtreeNode<T>* _right;
    RBtreeNode<T>* _parent;
    T _data;
    Color _color;

    explicit RBtreeNode(const T& data)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_data(data)
        ,_color(RED)
    {}
};

template <typename T, typename Ptr, typename Ref>
struct __RBtreeiterator
{
    typedef RBtreeNode<T> Node;
    typedef __RBtreeiterator<T, Ptr, Ref> Self;
    Node* _node;

    __RBtreeiterator(Node* node)
        :_node(node)
    {}

    Ref operator*()
    {
        return _node->_data;
    }

    Ptr operator->()
    {
        return &_node->_data;
    }

    Self& operator++()
    {
        // 右子树不为空，则找右子树的最左节点
        if(_node->_right != nullptr)
        {
            _node = _node->_right;
            while(_node->_left)
            {
                _node = _node->_left;
            }
        }
        // 右子树为空，则向上找，直到找到一个节点是其父节点的左孩子
        else
        {
            Node* parent = _node->_parent;
            while(parent != nullptr && _node == parent->_right)
            {
                _node = parent;
                parent = parent->_parent;
            }
            // 已经是中序最后一个节点时 parent 为 nullptr，即 end()
            _node = parent;
        }
        return *this;
    }

    bool operator!=(const Self& other) const
    {
        return _node != other._node;
    }
};

template <typename K, typename T, typename KeyOfT>
class RBtree
{
public:
    typedef RBtreeNode<T> Node;
    // 类模版一致，但实例化迭代器类型不同
    typedef __RBtreeiterator<T, T*, T&> iterator;
    typedef __RBtreeiterator<T, const T*, const T&> const_iterator;

    // 最左节点
    iterator begin()
    {
        Node* cur = _root;
        if(cur == nullptr)
        {
            return iterator(nullptr);
        }
        while(cur->_left)
        {
            cur = cur->_left;
        }
        return iterator(cur);
    }

    const_iterator begin() const
    {
        Node* cur = _root;
        if(cur == nullptr)
        {
            return const_iterator(nullptr);
        }
        while(cur->_left)
        {
            cur = cur->_left;
        }
        return const_iterator(cur);
    }

    // 最右节点的下一个位置，即end()
    iterator end()
    {
        return iterator(nullptr);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    RBtree()
        :_root(nullptr)
    {}

    ~RBtree()
    {
        Destroy(_root);
    }

    RBtree(const RBtree&) = delete;
    RBtree& operator=(const RBtree&) = delete;

    Node* find(const K& key) const
    {
        Node* cur = _root;
        KeyOfT keyOfT;
        while(cur != nullptr)
        {
            const K& curKey = keyOfT(cur->_data);
            if(curKey < key)
            {
                cur = cur->_right;
            }
            else if(key < curKey)
            {
                cur = cur->_left;
            }
            else
            {
                return cur;
            }
        }
        return nullptr;
    }

    bool insert(const T& data)
    {
        Node* parent = nullptr;
        Node* cur = _root;
        KeyOfT keyOfT;

        while(cur != nullptr)
        {
            if(keyOfT(data) < keyOfT(cur->_data))
            {
                parent = cur;
                cur = cur->_left;
            }
            else if(keyOfT(cur->_data) < keyOfT(data))
            {
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                return false;
            }
        }

        Node* newNode = new Node(data);
        newNode->_parent = parent;
        if(parent == nullptr)
        {
            _root = newNode;
        }
        else if(keyOfT(data) < keyOfT(parent->_data))
        {
            parent->_left = newNode;
        }
        else
        {
            parent->_right = newNode;
        }

        Node* node = newNode;
        while(node != _root && node->_parent->_color == RED)
        {
            parent = node->_parent;
            Node* grandparent = parent->_parent;
            if(parent == grandparent->_left)
            {
                Node* uncle = grandparent->_right;
                if(uncle != nullptr && uncle->_color == RED)
                {
                    parent->_color = BLACK;
                    uncle->_color = BLACK;
                    grandparent->_color = RED;
                    node = grandparent;
                }
                else
                {
                    if(node == parent->_right)
                    {
                        node = parent;
                        RotateL(node);
                        parent = node->_parent;
                        grandparent = parent->_parent;
                    }
                    parent->_color = BLACK;
                    grandparent->_color = RED;
                    RotateR(grandparent);
                }
            }
            else
            {
                Node* uncle = grandparent->_left;
                if(uncle != nullptr && uncle->_color == RED)
                {
                    parent->_color = BLACK;
                    uncle->_color = BLACK;
                    grandparent->_color = RED;
                    node = grandparent;
                }
                else
                {
                    if(node == parent->_left)
                    {
                        node = parent;
                        RotateR(node);
                        parent = node->_parent;
                        grandparent = parent->_parent;
                    }
                    parent->_color = BLACK;
                    grandparent->_color = RED;
                    RotateL(grandparent);
                }
            }
        }

        _root->_color = BLACK;
        return true;
    }

    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        parent->_right = subRL;
        if(subRL != nullptr)
        {
            subRL->_parent = parent;
        }
        subR->_parent = parent->_parent;
        if(parent->_parent == nullptr)
        {
            _root = subR;
        }
        else if(parent == parent->_parent->_left)
        {
            parent->_parent->_left = subR;
        }
        else
        {
            parent->_parent->_right = subR;
        }
        subR->_left = parent;
        parent->_parent = subR;
    }

    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        parent->_left = subLR;
        if(subLR != nullptr)
        {
            subLR->_parent = parent;
        }
        subL->_parent = parent->_parent;
        if(parent->_parent == nullptr)
        {
            _root = subL;
        }
        else if(parent == parent->_parent->_left)
        {
            parent->_parent->_left = subL;
        }
        else
        {
            parent->_parent->_right = subL;
        }
        subL->_right = parent;
        parent->_parent = subL;
    }

    void InOrder() const
    {
        InOrder(_root);
        std::cout << std::endl;
    }

    void PrintTree() const
    {
        if(_root == nullptr)
        {
            std::cout << "(empty tree)" << std::endl;
            return;
        }
        std::cout << KeyOfT()(_root->_data)
                  << (_root->_color == RED ? "(R)" : "(B)") << std::endl;
        PrintTree(_root->_left, "", true);
        PrintTree(_root->_right, "", false);
    }

    bool isRBTree() const
    {
        if(_root == nullptr)
        {
            return true;
        }
        if(_root->_color != BLACK || _root->_parent != nullptr)
        {
            return false;
        }
        int blackNum = -1;
        return Check(_root, nullptr, nullptr, 0, blackNum);
    }

    std::size_t size() const
    {
        return size(_root);
    }

private:
    void InOrder(Node* node) const
    {
        if(node == nullptr)
        {
            return;
        }
        InOrder(node->_left);
        std::cout << KeyOfT()(node->_data)
                  << (node->_color == RED ? "(R) " : "(B) ");
        InOrder(node->_right);
    }

    void PrintTree(Node* node, const std::string& prefix, bool isLeft) const
    {
        if(node == nullptr)
        {
            return;
        }
        std::cout << prefix << (isLeft ? "|-- " : "`-- ")
                  << KeyOfT()(node->_data)
                  << (node->_color == RED ? "(R)" : "(B)") << std::endl;
        std::string childPrefix = prefix + (isLeft ? "|   " : "    ");
        PrintTree(node->_left, childPrefix, true);
        PrintTree(node->_right, childPrefix, false);
    }

    bool Check(Node* node, const K* minKey, const K* maxKey,
               int blackCount, int& blackNum) const
    {
        if(node == nullptr)
        {
            if(blackNum == -1)
            {
                blackNum = blackCount;
            }
            return blackCount == blackNum;
        }
        if(node->_color == RED && node->_parent->_color == RED)
        {
            return false;
        }
        if(node->_left != nullptr && node->_left->_parent != node)
        {
            return false;
        }
        if(node->_right != nullptr && node->_right->_parent != node)
        {
            return false;
        }

        const K& key = KeyOfT()(node->_data);
        if(minKey != nullptr && !(*minKey < key))
        {
            return false;
        }
        if(maxKey != nullptr && !(key < *maxKey))
        {
            return false;
        }

        int curBlackCount = blackCount + (node->_color == BLACK ? 1 : 0);
        return Check(node->_left, minKey, &key, curBlackCount, blackNum)
            && Check(node->_right, &key, maxKey, curBlackCount, blackNum);
    }

    std::size_t size(Node* node) const
    {
        if(node == nullptr)
        {
            return 0;
        }
        return 1 + size(node->_left) + size(node->_right);
    }

    void Destroy(Node* node)
    {
        if(node == nullptr)
        {
            return;
        }
        Destroy(node->_left);
        Destroy(node->_right);
        delete node;
    }

    Node* _root;
};
