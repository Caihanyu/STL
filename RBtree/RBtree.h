#pragma once
#include <iostream>
#include <cstddef>
#include <string>
using namespace std;

enum Color
{
    RED,
    BLACK
};

template <typename K, typename V>
struct RBtreeNode
{
    RBtreeNode<K, V>* _left;
    RBtreeNode<K, V>* _right;
    RBtreeNode<K, V>* _parent;
    pair<K, V> _kv;
    Color _color;
    RBtreeNode(const pair<K, V>& kv = pair<K, V>())
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_kv(kv)
        ,_color(RED)
    {}   
};

template <typename K, typename V>
class RBtree
{
    typedef RBtreeNode<K, V> Node;
public:
    RBtree()
        :_root(nullptr)
    {
        // 创建哨兵节点 _NIL：所有空指针位置都指向它，且颜色恒为黑
        _NIL = new Node;
        _NIL->_color = BLACK;
        _NIL->_left = _NIL;
        _NIL->_right = _NIL;
        _NIL->_parent = _NIL;
        // 空树时，_root 指向哨兵
        _root = _NIL;
    }

    ~RBtree()
    {
        Destroy(_root);
        delete _NIL;
        _root = nullptr;
        _NIL = nullptr;
    }

    // 节点是裸指针，默认拷贝会让两棵树共享同一批节点（析构时双重释放），先禁用
    RBtree(const RBtree&) = delete;
    RBtree& operator=(const RBtree&) = delete;

    bool insert(const pair<K, V>& kv)
    {
        // 树是空的，节点为黑色
        if(_root == _NIL)
        {
            _root = new Node;
            _root->_kv = kv;
            _root->_color = BLACK;
            _root->_left = _NIL;
            _root->_right = _NIL;
            _root->_parent = _NIL;
            return true;
        }
        else
        {
            Node* cur = _root;
            Node* parent = _NIL;
            // 搜索树方法找到插入位置
            while(cur != _NIL)
            {
                if(kv.first < cur->_kv.first)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else if(kv.first > cur->_kv.first)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else
                {
                    return false;
                }
            }
            // 创建新节点
            Node* newNode = new Node;
            newNode->_kv = kv;
            newNode->_color = RED;
            newNode->_left = _NIL;
            newNode->_right = _NIL;
            newNode->_parent = parent;
            // 将新节点插入到父节点的左子树或右子树（根据 key 判断挂在哪边）
            if(kv.first < parent->_kv.first)
            {
                parent->_left = newNode;
            }
            else
            {
                parent->_right = newNode;
            }
            // cur重新赋值为新节点，不是原来位置上的NIL节点,同时更新parent和grandparent
            cur = newNode;
            parent = cur->_parent;
            // 进行红黑树的调整
            // 只要父节点是红色，就需要进行调整
            while(parent && parent->_color == RED)
            {
                // 获取祖父节点
                Node* grandparent = parent->_parent;
                // 判断父节点是祖父节点的左子树还是右子树
                if(grandparent->_left == parent)
                {
                    // 父节点为祖父节点的左子树
                    Node* uncle = grandparent->_right;
                    // 情况1：叔叔节点是红色
                    if(uncle && uncle->_color == RED)
                    {
                        // 父节点和叔叔节点变为黑色，祖父节点变成红色
                        parent->_color = BLACK;
                        uncle->_color = BLACK;
                        grandparent->_color = RED;
                        // 将祖父节点作为当前节点，继续向上调整
                        cur = grandparent;
                        parent = cur->_parent;
                    }
                    else if(uncle->_color == BLACK || uncle == _NIL)
                    {
                        // 情况2：叔叔节点不存在或为黑色，且cur节点是父节点的右子树
                        if(parent->_right == cur)
                        {
                            // 此时左右情况，先进行左旋，再进行右旋
                            RotateL(parent);
                            RotateR(grandparent);
                            // 旋转后，cur节点变为黑色，祖父节点变为红色
                            cur->_color = BLACK;
                            grandparent->_color = RED;
                        }
                        // 情况3：叔叔节点不存在或为黑色，且cur节点是父节点的左子树
                        if(parent->_left == cur)
                        {
                            // 此时为左左情况，进行右旋
                            RotateR(grandparent);
                            // 旋转后，父节点变为黑色，祖父节点变为红色
                            parent->_color = BLACK;
                            grandparent->_color = RED;
                        }
                    }
                }
                else
                {
                    // 父节点为祖父节点的右子树
                    Node* uncle = grandparent->_left;
                    // 情况1：叔叔节点是红色
                    if(uncle && uncle->_color == RED)
                    {
                        // 父节点和叔叔节点变为黑色，祖父节点变成红色
                        parent->_color = BLACK;
                        uncle->_color = BLACK;
                        grandparent->_color = RED;
                        // 将祖父节点作为当前节点，继续向上调整
                        cur = grandparent;
                        parent = cur->_parent;
                    }
                    else if(uncle->_color == BLACK || uncle == _NIL)
                    {
                        // 情况2：叔叔节点不存在或为黑色，且cur节点是父节点的左子树
                        if(parent->_left == cur)
                        {
                            // 此时左右情况，先进行右旋，再进行左旋
                            RotateR(parent);
                            RotateL(grandparent);
                            // 旋转后，cur节点变为黑色，祖父节点变为红色
                            cur->_color = BLACK;
                            grandparent->_color = RED;
                        }
                        // 情况3：叔叔节点不存在或为黑色，且cur节点是父节点的右子树
                        if(parent->_right == cur)
                        {
                            // 此时为右右情况，进行左旋
                            RotateL(grandparent);
                            // 旋转后，父节点变为黑色，祖父节点变为红色
                            parent->_color = BLACK;
                            grandparent->_color = RED;
                        }
                    }
                }
            }
            // 根节点必须为黑色（情况1的调整可能把根节点染红）
            _root->_color = BLACK;
            return true;
        }
    }

    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        parent->_right = subRL;
        if(subRL != _NIL)
        {
            subRL->_parent = parent;
        }
        subR->_parent = parent->_parent;
        // 确定subR的位置
        if(parent->_parent == _NIL)
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
        // 让 parent 挂到 subR 的左孩子上，并更新 parent 的父亲指针
        subR->_left = parent;
        parent->_parent = subR;
    }

    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        parent->_left = subLR;
        if(subLR != _NIL)
        {
            subLR->_parent = parent;
        }
        subL->_parent = parent->_parent;
        // 确定subL的位置
        if(parent->_parent == _NIL)
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
        // 让 parent 挂到 subL 的右孩子上，并更新 parent 的父亲指针
        subL->_right = parent;
        parent->_parent = subL;
    }

    // ========== 以下是测试/调试辅助接口 ==========

    // 中序遍历打印（键 + 颜色 R/B），方便观察结构和红黑性质
    void InOrder() const
    {
        InOrder(_root);
        std::cout << std::endl;
    }

    // 树形结构打印（含颜色 R/B）：root 在最上面，
    // |-- 表示左孩子，`-- 表示右孩子，例如：
    //   7(B)
    //   |-- 3(R)
    //   `-- 16(R)
    void PrintTree() const
    {
        if(_root == _NIL)
        {
            std::cout << "(empty tree)" << std::endl;
            return;
        }
        std::cout << _root->_kv.first << (_root->_color == RED ? "(R)" : "(B)") << std::endl;
        PrintTree(_root->_left, "", true);
        PrintTree(_root->_right, "", false);
    }

    // 校验红黑树性质：
    // 1. 根是黑色；2. 没有连续的红节点；3. 每条路径黑节点数相同；4. 中序有序（搜索树性质）
    bool isRBTree() const
    {
        if(_root == _NIL)
        {
            return true;
        }
        if(_root->_color != BLACK)
        {
            return false;
        }
        if(_root->_parent != _NIL)
        {
            return false;
        }
        int blackNum = -1;   // 第一条路径的黑节点总数，作为基准
        return Check(_root, nullptr, nullptr, 0, blackNum);
    }

    // 节点个数
    size_t size() const
    {
        return size(_root);
    }

private:
    void InOrder(Node* node) const
    {
        if(node == _NIL)
        {
            return;
        }
        InOrder(node->_left);
        std::cout << node->_kv.first << (node->_color == RED ? "(R) " : "(B) ");
        InOrder(node->_right);
    }

    // 树形打印递归：prefix 是行前缀，isLeft 表示当前节点是左孩子还是右孩子
    void PrintTree(Node* node, const std::string& prefix, bool isLeft) const
    {
        if(node == _NIL)
        {
            return;
        }
        std::cout << prefix << (isLeft ? "|-- " : "`-- ")
                  << node->_kv.first << (node->_color == RED ? "(R)" : "(B)") << std::endl;
        // 当前节点是左孩子时，它的下方还会有右兄弟的分支，用竖线把层次延续下去
        std::string childPrefix = prefix + (isLeft ? "|   " : "    ");
        PrintTree(node->_left, childPrefix, true);
        PrintTree(node->_right, childPrefix, false);
    }

    bool Check(Node* node, const K* minKey, const K* maxKey, int blackCount, int& blackNum) const
    {
        // 走到哨兵说明一条路径走完了，用第一条路径的黑节点数做基准比较
        if(node == _NIL)
        {
            if(blackNum == -1)
            {
                blackNum = blackCount;
            }
            return blackCount == blackNum;
        }
        // 不能有连续的红节点
        if(node->_color == RED && node->_parent->_color == RED)
        {
            return false;
        }
        // 父子指针必须互相指对（旋转后容易漏掉回接）
        if(node->_left != _NIL && node->_left->_parent != node)
        {
            return false;
        }
        if(node->_right != _NIL && node->_right->_parent != node)
        {
            return false;
        }
        // 中序必须有序（用上下界约束检查）
        if(minKey != nullptr && !(*minKey < node->_kv.first))
        {
            return false;
        }
        if(maxKey != nullptr && !(node->_kv.first < *maxKey))
        {
            return false;
        }
        // 统计黑节点个数（含当前节点）
        int curBlackCount = blackCount + (node->_color == BLACK ? 1 : 0);
        return Check(node->_left, minKey, &node->_kv.first, curBlackCount, blackNum)
            && Check(node->_right, &node->_kv.first, maxKey, curBlackCount, blackNum);
    }

    size_t size(Node* node) const
    {
        if(node == _NIL)
        {
            return 0;
        }
        return 1 + size(node->_left) + size(node->_right);
    }

    // 递归释放所有真实节点（哨兵不在这里释放）
    void Destroy(Node* node)
    {
        if(node == _NIL)
        {
            return;
        }
        Destroy(node->_left);
        Destroy(node->_right);
        delete node;
    }



    Node* _root;
    Node* _NIL;
};