#pragma once

#include <deque>

namespace cai
{
    template <class T, class Container = std::deque<T>>
    class queue
    {
    private:
        Container _con;
    public:
        void push(const T& value)
        {
            _con.push_back(value);
        }

        void pop()
        {
            _con.pop_front();
        }

        T& front()
        {
            return _con.front();
        }

        size_t size() const
        {
            return _con.size();
        }

        bool empty() const
        {
            return _con.empty();
        }
    };
}

