#include <iostream>

template <class K>
struct BinarySearchTreeNode
{
    K _key;
    BinarySearchTreeNode *left;
    BinarySearchTreeNode *right;

    BinarySearchTreeNode(K key) : _key(key), left(nullptr), right(nullptr) {}
};


template <class K>
class BinarySearchTree
{
private:
    BinarySearchTreeNode<K> *root;
public:
    BinarySearchTree() : root(nullptr) {}

    void insert(K key)
    {
        _insert(root, key);
    }
    
    void erase(K key)
    {
        _erase(root, key);
    }

    void find(const K& key)
    {
        _find(root, key);
    }

    void inorder()
    {
        _inorder(root);
    }
private:
    void _insert(BinarySearchTreeNode<K>*& node, K key)
    {
        // 空节点直接插入
        if (node == nullptr)
        {
            node = new BinarySearchTreeNode<K>(key);
        }
        else
        {
            // 递归插入
            if (key < node->_key)
            {
                _insert(node->left, key);
            }
            else if (key > node->_key)
            {
                _insert(node->right, key);
            }
        }
    }

    void _erase(BinarySearchTreeNode<K>*& node, K key)
    {
        // 先找节点
        if (node == nullptr)
        {
            return;
        }
        if (key < node->_key)
        {
            _erase(node->left, key);
        }
        else if (key > node->_key)
        {
            _erase(node->right, key);
        }
        else
        {
            // 找到要删除的节点
            if (node->left == nullptr && node->right == nullptr)
            {
                // 叶子节点，直接删除
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr)
            {
                // 只有右子树，直接用右子树替代
                BinarySearchTreeNode<K>* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr)
            {
                // 只有左子树，直接用左子树替代
                BinarySearchTreeNode<K>* temp = node;
                node = node->left;
                delete temp;
            }
            else
            {
                // 有两个子树，找到右子树的最小值替代当前节点(或者左子树的最大值也可以)
                BinarySearchTreeNode<K>* minNode = _findMin(node->right);
                node->_key = minNode->_key;
                _erase(node->right, minNode->_key);
            }
        }
    }

    void _find(BinarySearchTreeNode<K>* node, const K& key)
    {
        if (node == nullptr)
        {
            std::cout << "Not found: " << key << std::endl;
            return;
        }
        if (key == node->_key)
        {
            std::cout << "Found: " << key << std::endl;
            return;
        }
        else if (key < node->_key)
        {
            _find(node->left, key);
        }
        else
        {
            _find(node->right, key);
        }
    }

    // 查找最小值，用于删除节点时替代
    BinarySearchTreeNode<K>* _findMin(BinarySearchTreeNode<K>* node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    // 中序遍历，看是否正确插入
    void _inorder(BinarySearchTreeNode<K> *node)
    {
        if (node != nullptr)
        {
            _inorder(node->left);
            std::cout << node->_key << " ";
            _inorder(node->right);
        }
    }
};

