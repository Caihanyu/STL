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

void bsttest3()
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

    BinarySearchTree<int> bstCopy(bst); // 使用复制构造函数
    std::cout << "Inorder traversal of copied tree: ";
    bstCopy.inorder(); // 输出应该是 2 3 4 5 6 7 8
    std::cout << std::endl;
}

void bsttest4()
{
    BinarySearchTree<int> bst1;
    bst1.insert(5);
    bst1.insert(3);
    bst1.insert(7);

    BinarySearchTree<int> bst2;
    bst2.insert(10);
    bst2.insert(8);
    bst2.insert(12);

    std::cout << "Inorder traversal of bst1: ";
    bst1.inorder(); // 输出应该是 3 5 7
    std::cout << std::endl;

    std::cout << "Inorder traversal of bst2: ";
    bst2.inorder(); // 输出应该是 8 10 12
    std::cout << std::endl;

    bst1 = bst2; // 使用赋值运算符重载

    std::cout << "Inorder traversal of bst1 after assignment: ";
    bst1.inorder(); // 输出应该是 8 10 12
    std::cout << std::endl;
}

int main()
{
    bsttest4();
    std::cin.get();
    return 0;
}