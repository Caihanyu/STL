#include "binarysearchtree.h"

void bsttest1()
{
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(2);
    bst.insert(4);
    bst.insert(6);
    bst.insert(8);

    std::cout << "Inorder traversal: ";
    bst.inorder(); // 输出应该是 2 3 4 5 6 7 8
    std::cout << std::endl;
    bst.find(4); // 输出 Found: 4
    bst.find(10); // 输出 Not found: 10
}

void bsttest2()
{
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(2);
    bst.insert(4);
    bst.insert(6);
    bst.insert(8);

    std::cout << "Inorder traversal: ";
    bst.inorder(); // 输出应该是 2 3 4 5 6 7 8
    std::cout << std::endl;
    bst.find(4); // 输出 Found: 4
    bst.erase(4);
    bst.find(4); // 输出 Not found: 4
    std::cout << "Inorder traversal after deleting 4: ";
    bst.inorder(); // 输出应该是 2 3 5 6 7 8
    std::cout << std::endl;
}

int main()
{
    bsttest2();
    std::cin.get();
    return 0;
}