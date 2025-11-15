#include"vector.h"

void vectortest1()
{
    cai::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    v.insert(v.end(), 100);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
}

int main()
{
    vectortest1();
    return 0;
}
