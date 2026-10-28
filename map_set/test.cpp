#include "map.h"
#include "set.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

static int g_pass = 0;
static int g_fail = 0;

void check(bool condition, const std::string& name)
{
    if(condition)
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

struct NonDefaultValue
{
    explicit NonDefaultValue(int value)
        :value(value)
    {}

    int value;
};

void test1()
{
    std::cout << "===== test1: set insert and tree properties =====" << std::endl;
    cai::set<int> values;
    int keys[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for(int key : keys)
    {
        check(values.insert(key), "set insert " + std::to_string(key));
        check(values.isRBTree(), "set RB property after insert " + std::to_string(key));
    }
    check(values.size() == 9, "set size == 9");
    for(int key : keys)
    {
        check(values.find(key), "set finds " + std::to_string(key));
    }
    check(!values.find(100), "set does not find missing key");
}

void test2()
{
    std::cout << "===== test2: duplicate set keys =====" << std::endl;
    cai::set<int> values;
    check(values.insert(50), "first set insert 50");
    check(!values.insert(50), "duplicate set key rejected");
    check(values.size() == 1, "set size stays 1");
    check(values.find(50), "set still finds 50");
    check(values.isRBTree(), "set tree remains valid after duplicate");
}

void test3()
{
    std::cout << "===== test3: ordered set inserts =====" << std::endl;
    cai::set<int> ascending;
    cai::set<int> descending;
    bool inserted = true;
    for(int i = 1; i <= 200; ++i)
    {
        inserted = ascending.insert(i) && inserted;
        inserted = descending.insert(201 - i) && inserted;
    }
    check(inserted, "ascending and descending inserts succeed");
    check(ascending.size() == 200 && descending.size() == 200,
          "both set sizes == 200");
    check(ascending.isRBTree(), "ascending set is a valid RB tree");
    check(descending.isRBTree(), "descending set is a valid RB tree");
}

void test4()
{
    std::cout << "===== test4: random set inserts vs std::set =====" << std::endl;
    std::mt19937 rng(20261010);
    std::uniform_int_distribution<int> dist(0, 50000);
    cai::set<int> values;
    std::set<int> reference;
    bool same = true;

    for(int i = 0; i < 20000; ++i)
    {
        int key = dist(rng);
        if(values.insert(key) != reference.insert(key).second)
        {
            same = false;
            break;
        }
        if((i + 1) % 4000 == 0 && !values.isRBTree())
        {
            same = false;
            break;
        }
    }

    check(same, "set insert results match std::set");
    check(values.size() == reference.size(), "set size matches std::set");
    check(values.isRBTree(), "randomly populated set is a valid RB tree");
    bool allFound = true;
    for(std::set<int>::const_iterator it = reference.begin(); it != reference.end(); ++it)
    {
        if(!values.find(*it))
        {
            allFound = false;
            break;
        }
    }
    check(allFound, "set finds every key in std::set");
}

void test5()
{
    std::cout << "===== test5: map insert and duplicate keys =====" << std::endl;
    cai::map<int, NonDefaultValue> values;
    check(values.insert(std::make_pair(7, NonDefaultValue(42))),
          "map inserts a non-default-constructible value");
    check(!values.insert(std::make_pair(7, NonDefaultValue(99))),
          "map rejects a duplicate key");
    check(values.size() == 1, "map size stays 1");
    check(values.find(7), "map finds inserted key");
    check(!values.find(8), "map does not find missing key");
    check(values.isRBTree(), "map tree is a valid RB tree");
}

void test6()
{
    std::cout << "===== test6: RBtree iterator operator++ =====" << std::endl;

    typedef std::pair<int, int> Value;
    struct IntKeyOfT
    {
        const int& operator()(const Value& kv) const
        {
            return kv.first;
        }
    };
    typedef RBtree<int, Value, IntKeyOfT> Tree;
    typedef RBtreeNode<Value> Node;
    typedef __RBtreeiterator<Value, Value*, Value&> Iterator;

    int keys[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    const int count = sizeof(keys) / sizeof(keys[0]);
    Tree tree;
    for(int i = 0; i < count; ++i)
    {
        tree.insert(Value(keys[i], keys[i] * 10));
    }

    std::vector<int> expected(keys, keys + count);
    std::sort(expected.begin(), expected.end());

    // 从最小节点开始，不断 ++ 应当正好按升序访问完所有节点
    Node* first = tree.find(expected.front());
    check(first != nullptr, "iterator can start at the smallest node");

    Iterator it(first);
    std::vector<int> visited;
    int steps = 0;
    const int limit = count + 10;
    while(it._node != nullptr && steps <= limit)
    {
        visited.push_back(it->first);
        ++it;
        ++steps;
    }
    check(visited == expected, "operator++ visits every node in ascending order");
    check(it._node == nullptr, "operator++ on the last node reaches end()");

    // 只有根节点的树，++ 之后也应当到达 end()
    Tree single;
    single.insert(Value(1, 1));
    Iterator one(single.find(1));
    ++one;
    check(one._node == nullptr, "operator++ on the only node reaches end()");
}

void test7()
{
    std::cout << "===== test7: set and map iterators =====" << std::endl;

    // ---------- set ----------
    cai::set<int> values;
    std::set<int> referenceSet;
    int keys[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
    for(int key : keys)
    {
        values.insert(key);
        referenceSet.insert(key);
    }

    std::vector<int> visited;
    for(cai::set<int>::iterator it = values.begin(); it != values.end(); ++it)
    {
        visited.push_back(*it);
    }
    std::vector<int> expected(referenceSet.begin(), referenceSet.end());
    check(visited == expected, "set iterator visits all keys in ascending order");
    check(visited.size() == values.size(),
          "set iterator visits every node exactly once");

    cai::set<int> emptySet;
    check(!(emptySet.begin() != emptySet.end()),
          "empty set: begin() equals end()");

    cai::set<int> singleSet;
    singleSet.insert(42);
    cai::set<int>::iterator singleIt = singleSet.begin();
    check(singleIt != singleSet.end() && *singleIt == 42,
          "single-element set: begin() dereferences to the only key");
    ++singleIt;
    check(!(singleIt != singleSet.end()),
          "single-element set: operator++ reaches end()");

    int sum = 0;
    for(int key : values)
    {
        sum += key;
    }
    int expectedSum = 0;
    for(int key : referenceSet)
    {
        expectedSum += key;
    }
    check(sum == expectedSum, "set works with range-based for");

    // ---------- map ----------
    cai::map<int, std::string> dict;
    std::map<int, std::string> referenceMap;
    const char* words[] = {"watermelon", "apple", "banana", "pear", "grape"};
    int mapKeys[] = {5, 1, 4, 2, 3};
    const int mapCount = sizeof(mapKeys) / sizeof(mapKeys[0]);
    for(int i = 0; i < mapCount; ++i)
    {
        dict.insert(std::make_pair(mapKeys[i], std::string(words[i])));
        referenceMap[mapKeys[i]] = words[i];
    }

    std::vector<int> keyOrder;
    bool pairsMatch = true;
    for(cai::map<int, std::string>::iterator it = dict.begin(); it != dict.end(); ++it)
    {
        keyOrder.push_back(it->first);
        // operator-> 与 operator* 必须指向同一个 pair
        if(it->first != (*it).first || it->second != (*it).second)
        {
            pairsMatch = false;
        }
        if(it->second != referenceMap[it->first])
        {
            pairsMatch = false;
        }
    }
    std::vector<int> expectedKeyOrder = {1, 2, 3, 4, 5};
    check(keyOrder == expectedKeyOrder, "map iterator visits keys in ascending order");
    check(pairsMatch, "map iterator-> / * expose the same pair as std::map");
    check(keyOrder.size() == dict.size(),
          "map iterator visits every node exactly once");

    cai::map<int, std::string> emptyMap;
    check(!(emptyMap.begin() != emptyMap.end()),
          "empty map: begin() equals end()");

    // 通过迭代器修改 value 应当写回树中
    for(cai::map<int, std::string>::iterator it = dict.begin(); it != dict.end(); ++it)
    {
        it->second += "!";
    }
    bool allUpdated = true;
    for(cai::map<int, std::string>::iterator it = dict.begin(); it != dict.end(); ++it)
    {
        if(it->second != referenceMap[it->first] + "!")
        {
            allUpdated = false;
            break;
        }
    }
    check(allUpdated, "map iterator allows modifying the mapped value");

    cai::map<int, std::string>::iterator lastIt = dict.begin();
    for(; lastIt != dict.end(); ++lastIt)
    {}
    check(!(lastIt != dict.end()), "walking a map with ++ finishes at end()");

    // ---------- 随机数据：迭代顺序与标准容器一致 ----------
    std::mt19937 rng(20261020);
    std::uniform_int_distribution<int> dist(0, 100000);
    cai::set<int> randomSet;
    std::set<int> randomRefSet;
    cai::map<int, int> randomMap;
    std::map<int, int> randomRefMap;
    for(int i = 0; i < 5000; ++i)
    {
        int key = dist(rng);
        randomSet.insert(key);
        randomRefSet.insert(key);
        randomMap.insert(std::make_pair(key, i));
        if(randomRefMap.find(key) == randomRefMap.end())
        {
            randomRefMap[key] = i;
        }
    }

    std::vector<int> customOrder;
    for(cai::set<int>::iterator it = randomSet.begin(); it != randomSet.end(); ++it)
    {
        customOrder.push_back(*it);
    }
    std::vector<int> stdOrder(randomRefSet.begin(), randomRefSet.end());
    check(customOrder == stdOrder,
          "random set: iterator order matches std::set");

    bool sameMap = true;
    std::map<int, int>::iterator refIt = randomRefMap.begin();
    for(cai::map<int, int>::iterator it = randomMap.begin(); it != randomMap.end(); ++it, ++refIt)
    {
        if(refIt == randomRefMap.end()
           || it->first != refIt->first
           || it->second != refIt->second)
        {
            sameMap = false;
            break;
        }
    }
    check(sameMap && refIt == randomRefMap.end(),
          "random map: iterator order and values match std::map");
}

void test8()
{
    std::cout << "===== test8: set and map element mutability =====" << std::endl;

    using SetReference = decltype(*std::declval<cai::set<int>::iterator&>());
    static_assert(
        std::is_const<typename std::remove_reference<SetReference>::type>::value,
        "set elements must not be modifiable through an iterator");

    using MapReference = decltype(*std::declval<cai::map<int, int>::iterator&>());
    static_assert(
        std::is_const<typename std::remove_reference<
            decltype((std::declval<MapReference>().first))>::type>::value,
        "map keys must not be modifiable through an iterator");
    static_assert(
        !std::is_const<typename std::remove_reference<
            decltype((std::declval<MapReference>().second))>::type>::value,
        "map values must be modifiable through an iterator");

    cai::map<int, int> values;
    values.insert(std::make_pair(1, 10));
    values.begin()->second = 20;
    std::cout << "map value after modifying second: " << values.begin()->second
              << std::endl;
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    std::cout << "========================================" << std::endl;
    std::cout << "passed: " << g_pass << ", failed: " << g_fail << std::endl;
    return g_fail == 0 ? 0 : 1;
}
