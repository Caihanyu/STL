#pragma once

namespace cai
{
    template <class Iterator, class Ref, class Ptr>
    class Reverseiterator
    {
        typedef Reverseiterator<Iterator, Ref, Ptr> Self;
    private:
        Iterator _it;
    public:
        Reverseiterator(Iterator it) 
            : _it(it) 
        {}

        Ref operator*()
        {
            Iterator tmp = _it;
            return *--tmp;
        }

        Ptr operator->()
        {
            return &operator*();
        }

        Self& operator++()
        {
            --_it;
            return *this;
        }

        Self& operator++(int)
        {
            --_it;
            return *this;
        }

        Self& operator--()
        {
            ++_it;
            return *this;
        }

        Self& operator--(int)
        {
            ++_it;
            return *this;
        }

        bool operator!=(const Self& s) const
        {
            return _it != s._it;
        }

        
    };
}