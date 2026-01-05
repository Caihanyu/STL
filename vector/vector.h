#include <iostream>
#include <assert.h>

namespace cai
{
    template <class T>
    class vector
    {
        public:
            typedef T* iterator;
            typedef const T* const_iterator;

            iterator begin()
            {
                return _start;
            }

            iterator end()
            {
                return _finish;
            }

            const_iterator begin() const
            {
                return _start;
            }

            const_iterator end() const
            {
                return _finish;
            }
        vector()
            :_start(nullptr)
            ,_finish(nullptr)
            ,_endofstorage(nullptr)
        {}

        vector(const vector<T>& v)
            :_start(nullptr)
            ,_finish(nullptr)
            ,_endofstorage(nullptr)
        {
            _start = new T[v.capacity()];
            memcpy(_start, v._start, sizeof(T) * v.size());
            _finish = _start + v.size();
            _endofstorage = _start + v.capacity();
        }

        void swap(vector<T> v)
        {
            std::swap(_start, v._start);
            std::swap(_finish, v._finish);
            std::swap(_endofstorage, v._endofstorage);
        }

        // 利用传值深拷贝，v1 = v2
        vector<T>& operator=(vector<T> v)
        {
            swap(v);
            return *this;
        }

        ~vector()
        {
            if(_start)
            {
                delete[] _start;
                _start = _finish = _endofstorage = nullptr;
            }
        }

        size_t capacity()
        {
            return _endofstorage - _start;
        }

        size_t size() const
        {
            return _finish - _start;
        }

        size_t capacity() const
        {
            return _endofstorage - _start;
        }

        size_t size()
        {
            return _finish - _start;
        }

        T& operator[](size_t pos)
        {
            assert(pos < size());
            return _start[pos];
        }

        T& operator[](size_t pos) const
        {
            assert(pos < size());
            return _start[pos];
        }

        // 增删查改

        void reserve(size_t newcapapcity)
        {
            // 要检查新空间大小，避免多开和重复开空间
            if(newcapapcity > capacity())
            {
                // 保存原来的的size
                size_t sz = size();
                // 开空间并复制原来的数据
                T* tmp = new T[newcapapcity];
                memcpy(tmp, _start, sizeof(T) * size());
                // 释放原空间并重定位_start
                delete[] _start;
                _start = tmp;
                // DEBUG：存在问题，计算size()这时候是拿原来的finish-当前的start，出现问题，需要保存原来的
                _finish = _start + sz;
                _endofstorage = _start + newcapapcity; 
            }
        }

        void resize(size_t n, const T& val = T())
        {
            // 两种空间情况：比原来短，比原来长
                if(n < size())
                {
                    _finish = _start + n;
                }
                else
                {
                    reserve(n);
                    while(_finish != _start + n)
                    {
                        *_finish = val;
                        ++_finish;
                    }
                }
        }

        void push_back(const T& val)
        {
            /*
            if(_finish == _endofstorage)
            {
                size_t newcapaity = capacity() == 0 ? 4 : capacity() * 2;
                reserve(newcapaity);
            }
            // 用的new开的空间，直接在_finish位置赋值就好了
            *_finish = val;
            _finish++;
            */
            insert(end(), val);
        }

        void pop_back()
        {
            erase(end());
        }

        // 要返回pos位置的迭代器
        iterator insert(iterator pos, const T& val)
        {
            // 扩容导致迭代器失效，需要更新迭代器
            if(_finish == _endofstorage)
            {
                // 保存相对位置
                size_t len = pos - _start;
                size_t newcapaity = capacity() == 0 ? 4 : capacity() * 2;
                reserve(newcapaity);
                // 更新迭代器(迭代器失效问题)
                pos = _start + len;
            }
            iterator end = _finish - 1;
            while(end >= pos)
            {
                *(end + 1) = *end;
                end--;
            }
            *pos = val;
            _finish++;
            return pos;
        }
        // 这里erase理论上迭代器不会失效，但最好还是认为迭代器失效，防止出现缩容情况
        iterator erase(iterator pos)
        {
            assert(pos >= _start && pos < _finish);
            iterator cur = pos + 1;
            while(cur != _finish)
            {
                *(cur - 1) = *cur;
                ++cur;
            }
            --_finish;
            return pos;
        }

        private:
            iterator _start;
            iterator _finish;
            iterator _endofstorage;
    };
}