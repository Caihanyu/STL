#include "my_string.h"

int main()
{
    cai::string s1("hello world");
    cout << s1.c_str() << endl;
    cai::string s2 = s1.substr(0, 5);
    cout << s2.c_str() << endl;
    return 0;
}