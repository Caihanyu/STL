#pragma once

#include <iostream>
#include <utility>
#include <algorithm>
#include <cassert>

// AVL 树节点：_bf = 右子树高度 - 左子树高度
template <typename K, typename V>
struct AVLTreeNode
{
    std::pair<K, V> _data;
    AVLTreeNode* _left;
    AVLTreeNode* _right;
    AVLTreeNode* _parent;
    int _bf;

    AVLTreeNode(const std::pair<K, V>& data)
        :_data(data)
        ,_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_bf(0)
    {}
};

template <typename K, typename V>
class AVLTree
{
    typedef AVLTreeNode<K, V> Node;
public:
    AVLTree()
        :_root(nullptr)
    {}

    ~AVLTree()
    {
        destroy(_root);
        _root = nullptr;
    }

    // 节点里是裸指针，默认拷贝会让两棵树共用节点（最终双重释放），这里直接禁用
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    // 获取节点子树高度
    int getHeight(Node* node) const
    {
        if(node == nullptr)
            return 0;
        int leftHeight = getHeight(node->_left);
        int rightHeight = getHeight(node->_right);
        return std::max(leftHeight, rightHeight) + 1;
    }

    // 整棵树的高度
    int height() const { return getHeight(_root); }

    // 中序遍历（顺便打印每个节点的 _bf，方便观察平衡情况）
    void InOrder() const
    {
        InOrder(_root);
        std::cout << std::endl;
    }

    // 校验每个节点的 _bf 是否真的等于「右子树高度 - 左子树高度」
    bool isBalance() const { return isBalance(_root); }

    bool insert(const std::pair<K, V>& data)
    {
        // 如果根节点为空，则直接插入
        if(_root == nullptr)
        {
            _root = new Node(data);
            return true;
        }
        // 否则，找到合适的位置插入，同时记下新节点的父亲
        Node* cur = _root;
        Node* parent = nullptr;
        Node* newnode = nullptr;    // 循环里挂上去的新节点
        while(cur)
        {
            if(data.first < cur->_data.first)
            {
                if(cur->_left == nullptr)
                {
                    newnode = new Node(data);
                    newnode->_parent = cur;
                    cur->_left = newnode;
                    parent = cur;
                    break;          // 注意：这里只能 break 跳出查找，不能 return（还要向上更新 _bf）
                }
                cur = cur->_left;
            }
            else if(data.first > cur->_data.first)
            {
                if(cur->_right == nullptr)
                {
                    newnode = new Node(data);
                    newnode->_parent = cur;
                    cur->_right = newnode;
                    parent = cur;
                    break;
                }
                cur = cur->_right;
            }
            else
            {
                return false; // 不允许插入重复的键
            }
        }

        // 从 parent 开始向上更新平衡因子_bf（右子树高度 - 左子树高度 = _bf），谁失衡就在谁那里旋转
        // child 表示「刚刚长高了 1」的那棵子树的根，用来判断新节点落在哪一侧
        Node* child = newnode;
        cur = parent;
        while(cur)
        {
            if(cur->_left == child)
                cur->_bf--;     // 左边长高 -> bf 减 1
            else
                cur->_bf++;     // 右边长高 -> bf 加 1

            if(cur->_bf == 0)
            {
                // 原来只有一边高，插入把它补齐了：这棵子树高度没变，祖先全都不用改
                break;
            }
            else if(cur->_bf == 1 || cur->_bf == -1)
            {
                // 这棵子树长高了 1，继续往上看
                child = cur;
                cur = cur->_parent;
            }
            else
            {
                // cur->_bf == ±2，cur 失衡，旋转
                assert(cur->_bf == 2 || cur->_bf == -2);

                // -2 表示左子树高度大于右子树高度
                if(cur->_bf == -2)
                {
                    // cur 的左孩子的左子树更高：左左 -> 右旋
                    if(cur->_left->_bf == -1)
                    {
                        Node* newroot = rightRotate(cur);
                        cur->_bf = 0;
                        newroot->_bf = 0;
                    }
                    // cur 的左孩子的右子树更高：左右 -> 先左旋左孩子，再右旋 cur
                    else
                    {
                        Node* subL = cur->_left;
                        Node* subLR = subL->_right;
                        int subLRbf = subLR->_bf;   // 旋转前先记下来，它决定另外两个节点的 bf

                        leftRotate(subL);
                        rightRotate(cur);

                        subLR->_bf = 0;
                        subL->_bf = (subLRbf == 1) ? -1 : 0;
                        cur->_bf = (subLRbf == -1) ? 1 : 0;
                    }
                }
                // +2 表示右子树高度大于左子树高度
                else
                {
                    // cur 的右孩子的右子树更高：右右 -> 左旋
                    if(cur->_right->_bf == 1)
                    {
                        Node* newroot = leftRotate(cur);
                        cur->_bf = 0;
                        newroot->_bf = 0;
                    }
                    // cur 的右孩子的左子树更高：右左 -> 先右旋右孩子，再左旋 cur
                    else
                    {
                        Node* subR = cur->_right;
                        Node* subRL = subR->_left;
                        int subRLbf = subRL->_bf;

                        rightRotate(subR);
                        leftRotate(cur);

                        subRL->_bf = 0;
                        subR->_bf = (subRLbf == -1) ? 1 : 0;
                        cur->_bf = (subRLbf == 1) ? -1 : 0;
                    }
                }

                // 旋转后这棵子树的高度恢复到插入前的高度，上面的祖先 bf 不变，直接结束
                break;
            }
        }
        return true;
    }
private:
    // 左旋：把 node 的右孩子提上来当子树根（只改结构，不改 _bf），返回新的子树根
    Node* leftRotate(Node* node)
    {
        Node* subR = node->_right;
        Node* subRL = subR->_left;

        // 1. subR 的左子树改挂在 node 的右边
        node->_right = subRL;
        if(subRL)
            subRL->_parent = node;

        // 2. subR 顶替 node 原来的位置
        Node* pparent = node->_parent;
        subR->_left = node;
        node->_parent = subR;
        subR->_parent = pparent;

        if(pparent == nullptr)          // node 原来是整棵树的根
            _root = subR;
        else if(pparent->_left == node)
            pparent->_left = subR;
        else
            pparent->_right = subR;

        return subR;
    }

    // 右旋：把 node 的左孩子提上来当子树根（只改结构，不改 _bf），返回新的子树根
    Node* rightRotate(Node* node)
    {
        Node* subL = node->_left;
        Node* subLR = subL->_right;

        // 1. subL 的右子树改挂在 node 的左边
        node->_left = subLR;
        if(subLR)
            subLR->_parent = node;

        // 2. subL 顶替 node 原来的位置
        Node* pparent = node->_parent;
        subL->_right = node;
        node->_parent = subL;
        subL->_parent = pparent;

        if(pparent == nullptr)
            _root = subL;
        else if(pparent->_left == node)
            pparent->_left = subL;
        else
            pparent->_right = subL;

        return subL;
    }

    // 后序释放所有节点
    void destroy(Node* node)
    {
        if(node == nullptr)
            return;
        destroy(node->_left);
        destroy(node->_right);
        delete node;
    }

    // 递归校验：每个节点的 _bf 是否等于真实的高度差
    bool isBalance(Node* node) const
    {
        if(node == nullptr)
            return true;
        int leftHeight = getHeight(node->_left);
        int rightHeight = getHeight(node->_right);
        if(rightHeight - leftHeight != node->_bf)
        {
            std::cout << "节点 " << node->_data.first << " 的 _bf 错了：应该是 "
                      << rightHeight - leftHeight << "，实际是 " << node->_bf << std::endl;
            return false;
        }
        return isBalance(node->_left) && isBalance(node->_right);
    }

    void InOrder(Node* node) const
    {
        if(node == nullptr)
            return;
        InOrder(node->_left);
        std::cout << node->_data.first << "(_bf=" << node->_bf << ") ";
        InOrder(node->_right);
    }

    Node* _root = nullptr;

};