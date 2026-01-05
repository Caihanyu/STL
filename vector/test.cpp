#include"vector.h"

void vectortest1()
{
    cai::vector<int> v;
    v.push_back(1);
    v.push_back(2);
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
    std::cout << std::endl;
    auto it = v.begin();
    // 删除偶数
    while(it != v.end())
    {
        if(*it % 2 == 0)
        {
            it = v.erase(it);
        }
        else
            it++;
    }
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void vectortest2()
{
    cai::vector<int> v;
    v.resize(10, 0);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void vectortest3()
{
    cai::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    cai::vector<int> v1(v);
    for(auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void vectortest4();

int main()
{
    vectortest3();
    return 0;
}
