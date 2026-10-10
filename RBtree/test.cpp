#include "RBtree.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <random>

static int g_pass = 0;
static int g_fail = 0;

// 简易断言：输出 PASS/FAIL 并统计
void check(bool cond, const std::string& name)
{
    if(cond)
    {
        ++g_pass;
        std::cout << "[PASS] " << name << std::endl;
    }
    else
    {
        ++g_fail;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

// 测试1：基础插入，每插入一个节点就检查一次红黑树性质
void test1()
{
    std::cout << "===== test1: basic insert =====" << std::endl;
    RBtree<int, int> t;
    int a[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for(int key : a)
    {
        check(t.insert(std::make_pair(key, key)), "insert " + std::to_string(key));
        check(t.isRBTree(), "RB property after insert " + std::to_string(key));
    }
    std::cout << "InOrder(key:color): ";
    t.InOrder();
    check(t.size() == 9, "size == 9");
}

// 测试2：升序、降序插入（连续触发旋转）
void test2()
{
    std::cout << "===== test2: ordered insert =====" << std::endl;
    RBtree<int, int> t1;
    RBtree<int, int> t2;
    bool ok1 = true;
    bool ok2 = true;
    for(int i = 1; i <= 200; ++i)
    {
        ok1 = t1.insert(std::make_pair(i, i)) && ok1;              // 升序 1..200
        ok2 = t2.insert(std::make_pair(201 - i, 201 - i)) && ok2;  // 降序 200..1
    }
    check(ok1 && ok2, "ascending/descending 200 inserts ok");
    check(t1.isRBTree(), "ascending tree is valid RB");
    check(t2.isRBTree(), "descending tree is valid RB");
    check(t1.size() == 200 && t2.size() == 200, "both size == 200");
}

// 测试3：重复键不允许插入
void test3()
{
    std::cout << "===== test3: duplicate key =====" << std::endl;
    RBtree<int, int> t;
    check(t.insert(std::make_pair(50, 50)), "first insert 50");
    check(!t.insert(std::make_pair(50, 999)), "duplicate 50 rejected");
    t.insert(std::make_pair(30, 30));
    t.insert(std::make_pair(70, 70));
    check(!t.insert(std::make_pair(30, 999)), "duplicate 30 rejected");
    check(t.size() == 3, "size stays 3");
    check(t.isRBTree(), "tree still valid after duplicates");
}

// 测试4：随机数据与 std::set 对拍
void test4()
{
    std::cout << "===== test4: random vs std::set =====" << std::endl;
    std::mt19937 rng(20261010);
    std::uniform_int_distribution<int> dist(0, 50000);
    RBtree<int, int> t;
    std::set<int> s;
    bool same = true;
    for(int i = 0; i < 20000; ++i)
    {
        int key = dist(rng);
        bool rbtOk = t.insert(std::make_pair(key, key));
        bool setOk = s.insert(key).second;
        if(rbtOk != setOk)
        {
            same = false;
            std::cout << "insert #" << i << " key=" << key
                      << " mismatch: RBtree=" << rbtOk << ", set=" << setOk << std::endl;
            break;
        }
        // 每 4000 次全树检查一次性质，避免频繁遍历拖慢速度
        if((i + 1) % 4000 == 0 && !t.isRBTree())
        {
            same = false;
            std::cout << "RB property broken after insert #" << (i + 1) << std::endl;
            break;
        }
    }
    check(same, "20000 random inserts match std::set");
    check(t.size() == s.size(), "same node count as set (" + std::to_string(s.size()) + ")");
    check(t.isRBTree(), "final tree is valid RB");
}

// 测试5：打乱顺序插入 1..1000
void test5()
{
    std::cout << "===== test5: shuffled insert =====" << std::endl;
    std::vector<int> v;
    for(int i = 1; i <= 1000; ++i)
    {
        v.push_back(i);
    }
    std::mt19937 rng(42);
    std::shuffle(v.begin(), v.end(), rng);
    RBtree<int, int> t;
    bool ok = true;
    for(int key : v)
    {
        ok = t.insert(std::make_pair(key, key)) && ok;
    }
    check(ok, "1000 shuffled keys all inserted");
    check(t.isRBTree(), "valid RB after shuffled insert");
    check(t.size() == 1000, "size == 1000");
}

// 测试6：树形打印演示
void test6()
{
    std::cout << "===== test6: print tree =====" << std::endl;

    // 空树
    RBtree<int, int> empty;
    std::cout << "empty tree:" << std::endl;
    empty.PrintTree();

    // 经典混合序列，能看到旋转后的平衡结构
    RBtree<int, int> t;
    int a[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for(int key : a)
    {
        t.insert(std::make_pair(key, key));
    }
    std::cout << "tree built from {16,3,7,11,9,26,18,14,15}:" << std::endl;
    t.PrintTree();
    check(t.isRBTree(), "printed tree is valid RB");

    // 升序插入：展示红黑树靠旋转保持平衡（不会退化成单支）
    RBtree<int, int> t2;
    for(int i = 1; i <= 10; ++i)
    {
        t2.insert(std::make_pair(i, i));
    }
    std::cout << "tree built from 1..10 (ascending, shows rotations):" << std::endl;
    t2.PrintTree();
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    //test6();
    std::cout << "========================================" << std::endl;
    std::cout << "passed: " << g_pass << ", failed: " << g_fail << std::endl;
    return g_fail == 0 ? 0 : 1;
}