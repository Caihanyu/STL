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
        std::cout << *it << std::endl;
        ++it;
    }
}


int main(int argc, char const *argv[])
{
    testlist1();
    return 0;
}
