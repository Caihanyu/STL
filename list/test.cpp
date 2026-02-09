#include "list.h"

void testlist1()
{
    cai::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    cai::list<int>::iterator it = l.begin();
    while(it != l.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
    l.insert(++l.begin(), 0);
    it = l.begin();
    while(it != l.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
    std::cout << "Size: " << l.size() << std::endl;
}

void testlist2()
{
    cai::list<int> l;
    for(int i = 0; i < 10; ++i) {
        l.push_back(i);
    }
    cai::list<int>::iterator it = l.begin();
    while(it != l.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
    cai::list<int> l1(l);
    it = l1.begin();
    while(it != l1.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
    cai::list<int> l2 = l;
    it = l2.begin();
    while(it != l2.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    testlist2();
    return 0;
}
