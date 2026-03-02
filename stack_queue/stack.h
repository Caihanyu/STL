#pragma once

#include <deque>

namespace cai
{
    template <class T, class Container = std::deque<T>>
    class stack
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
            _con.pop_back();
        }

        T& top()
        {
            return _con.back();
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

