#include <iostream>
#include "priority_queue.h"

using namespace std;

int main()
{
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    cai::priority_queue<int, vector<int>, greater<int>> pq(arr, arr + sizeof(arr) / sizeof(int));
    while(!pq.empty())
    {
        cout << pq.top() << " ";
        pq.pop();
    }

    return 0;
}