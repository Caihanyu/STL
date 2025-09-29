#include "my_string.h"

int main()
{
    cai::string s1("hello ");
    s1.insert(6,"world");
    cout << s1.c_str() << endl;

    return 0;
}