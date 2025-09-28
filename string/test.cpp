#include "my_string.h"

int main()
{
    cai::string s1("hello world");
    s1.push_back('!');
    cout << s1.c_str() << endl;
    s1 += "this is a test";
    cout << s1.c_str() << endl;
    return 0;
}