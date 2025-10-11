#include "my_string.h"

void string_test1()
{
    cai::string s1("hello world");
    cout << s1.c_str() << endl;
    cai::string s2 = s1.substr(0, 5);
    cout << s2.c_str() << endl;
}

void string_test2()
{
    cai::string s1("hello world");
    s1.resize(20,'!');
    cout << s1 << endl;
    s1.resize(6);
    cout << s1 << endl;
    s1.resize(10);
    cout << s1 << endl;
}

void string_test3()
{
    cai::string s("hello");
    s += '\0';
    s += " world";
    // 以\0为结尾
    cout << s.c_str() << endl;
    // 以_size为结尾
    cout << s << endl;
}

int main()
{
    string_test3();
    return 0;
}