#include"vector.h"
#include <iostream>

using namespace std;

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

void vectortest4()
{
    /* vector<string>内部是自定义类型，浅拷贝会出问题（memcpy是浅拷贝）
    ，自定义类型必须深拷贝*/
    cai::vector<string> v;
    v.push_back("111");
    v.push_back("222");
    v.push_back("333");
    v.push_back("444");
    v.push_back("555");

    for (auto e : v)
    {
        cout << e << " ";
    }
    cout << endl;
}

void vectortest5()
{
    cai::vector<int> v(10,1);
    for (auto e : v)
    {
        cout << e << " ";
    }
    cout << endl;
    cai::vector<int> v1(v.begin(), v.end());
    for (auto e : v1)
    {
        cout << e << " ";
    }
}

int main()
{
    vectortest5();
    return 0;
}
