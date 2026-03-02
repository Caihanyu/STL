#include <iostream>

using namespace std;

#include "stack.h"
#include "queue.h"

void test_stack()
{
    cai::stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.push(3);
    cout << "size: " << s1.size() << endl;
    while(!s1.empty())
    {
        cout << s1.top() << " ";
        s1.pop();
    }
    cout << endl;
}

void test_queue()
{
    cai::queue<int> q1;
    q1.push(1);
    q1.push(2);
    q1.push(3);
    cout << "size: " << q1.size() << endl;
    while (!q1.empty())
    {
        cout << q1.front() << " ";
        q1.pop();
    }
    cout << endl;
}

int main()
{
    test_stack();
    test_queue(); 
    return 0;
}
