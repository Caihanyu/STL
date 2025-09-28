#pragma once
#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

namespace cai
{
    class string
    {
    private:
      char* _str;
      size_t _size;
      size_t _capacity; 
    public:
        typedef char* iterator;
        typedef const char* const_iterator;
        // 全缺省解决无参构造
        string(const char* str = "")
        // 初始化顺序和声明顺序保持一致，和初始化顺序无关
            :_str(new char[strlen(str) + 1])
            ,_size(strlen(str))
            ,_capacity(strlen(str))
        {
            strcpy(_str, str);
        }
        // 无参构造,见全缺省构造
        // string()
        // {
        //     _size = _capacity = 1;
        //     _str = new char(1);
        //     _str[0] = '\0';
        // }
        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }

        iterator begin()
        {
            return _str;
        }
        const_iterator begin() const 
        {
            return _str;
        }

        iterator end()
        {
            return _str + _size;
        }
        const_iterator end() const
        {
            return _str + _size;
        }

        // 字符串转换
        char* c_str() const
        {
            return _str;
        }

        size_t size() const
        {
            return _size;
        }

        void print()
        {
            cout << _str << endl;
            cout << "size = " << _size << endl;
            cout << "capacity = " << _capacity << endl;
        }

        char& operator[](size_t pos)
        {
            assert(pos < _size);
            return _str[pos];
        }
        char& operator[](size_t pos) const
        {
            assert(pos < _size);
            return _str[pos];
        }

        // 增删查改
        void reserve(size_t n = 0)
        {
            if(n > _capacity)
            {
                char* tmp = new char[n + 1];
                strcpy(tmp, _str);
                delete[] _str;
                _str = nullptr;
                _str = tmp;
                _capacity = n;
            }
        }

        void push_back(char c)
        {
            if(_size == _capacity)
            {
                reserve(_capacity == 0 ? 5 : _capacity + 1);
            }
            _str[_size] = c;
            _size++;
            _str[_size] = '\0';
        }

        string& append(const char* s)
        {
            if(_size + strlen(s) > _capacity)
            {
                reserve(_capacity + strlen(s));
            }
            strcpy(_str + _size, s);
            _size += strlen(s);
            return *this;
        }

        string& operator+=(const char* s)
        {
            append(s);
            return *this;
        }
    };
};

