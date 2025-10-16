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

      static size_t npos;
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
            //strcpy(_str, str);
            memcpy(_str, str, _size + 1);
        }
        // 深拷贝，防止值传递导致的野指针问题
        string(const string& str)
        {
            _str = new char[str._capacity + 1];
            _size = str._size;
            _capacity = str._capacity;
            //strcpy(_str,str._str);
            memcpy(_str, str._str, str._size + 1);
        }

        // 赋值 存在两种写法
        // 方案1
        // string& operator=(const string& s)
        // {
        //     // 判断是否是自身赋值
        //     if(this != &s)
        //     {
        //         delete[] this;
        //         char* tmp = new char[s._capacity + 1];
        //         memcpy(tmp, s._str, s._size + 1);
        //         _str = tmp;
        //         _size = s._size;
        //         _capacity = s._capacity;
        //     }
        //     return *this;
        // }
        
        // 方案2
        string& operator=(const string& s)
        {
            string tmp(s);
            // 这里可以单独做一个swap函数，STL也给了，但我偷懒~
            std::swap(_str, tmp._str);
            std::swap(_size, tmp._size);
            std::swap(_capacity, tmp._capacity);
            return *this;
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
        const char* c_str() const
        {
            return _str;
        }

        size_t size() const
        {
            return _size;
        }

        void clear()
        {
            _str[0] = '\0';
            _size = 0;
        }

        // void print()
        // {
        //     cout << _str << endl;
        //     cout << "size = " << _size << endl;
        //     cout << "capacity = " << _capacity << endl;
        // }

        // 比较大小
        bool operator==(const string& s) const
        {
            if(_size != s._size)
                return false;
            else
            {
                for (size_t i = 0; i < _size; i++)
                {
                    if(_str[i] != s._str[i])
                        return false;
                }
                return true;
                
            }
        }

        bool operator!=(const string& s) const
        {
            return !(*this == s);
        }

        bool operator<(const string& s) const
        {
            size_t i = 0;
            // 根据短的长度进行比较
            size_t shortlen = min(_size, s._size);
            for(; i < shortlen; i++)
            {
                if(_str[i] < s._str[i])
                    return true;
                else if(_str[i] > s._str[i])
                    return false;
            }
            // 前shortlen都相同的情况下比长度
            if(_size < s._size)
                return true;
            else
                return false;
        }

        bool operator>(const string& s) const
        {
            return !(*this < s);
        }

        bool operator<=(const string& s) const
        {
            return (*this < s) || (*this == s);
        }

        bool operator>=(const string& s) const
        {
            return (*this > s) || (*this == s);
        }

        // 下标定位
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
        void resize(size_t n, char ch = '\0')
        {
            if(n < _size)
            {
                _size = n;
                _str[_size] = '\0';
            }
            else
            {
                reserve(n);
                for (size_t i = _size; i < n; i++)
                {
                    _str[i] = ch;
                }
                _size = n;
                _str[_size] = '\0';
            }
        }

        void reserve(size_t n = 0)
        {
            if(n > _capacity)
            {
                char* tmp = new char[n + 1];
                //strcpy(tmp, _str);
                memcpy(tmp, _str, _size + 1);
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
            //strcpy(_str + _size, s);
            memcpy(_str + _size, s, strlen(s) + 1);
            _size += strlen(s);
            _str[_size] = '\0';
            return *this;
        }

        string& operator+=(const char* s)
        {
            append(s);
            return *this;
        }

        string& operator+=(const char ch)
        {
            push_back(ch);
            return *this;
        }

        string& insert(size_t pos, const char* str)
        {
            size_t len = strlen(str);
            assert(pos <= _size);
            if(_size + len > _capacity)
            {
                reserve(_size + len);
            }
            // size_t是无符号数，在0位置会出错（会出现无负数情况，-1是一个大数），故添加npos
            size_t end = _size;
            while(end >= pos && end != npos)
            {
                _str[end + len] = _str[end];
                end--;
            }
            for(int i = 0; i < len; i++)
            {
                _str[i + pos] = str[i];
            }
            _size += len;
            return *this;
        }

        string& insert(size_t pos, size_t n, char c)
        {
            assert(pos <= _size);
            if(_size + n > _capacity)
            {
                reserve(_size + n);
            }
            // size_t是无符号数，在0位置会出错，故添加npos
            size_t end = _size;
            while(end >= pos && end != npos)
            {
                _str[end + n] = _str[end];
                end--;
            }
            for(int i = 0; i < n; i++)
            {
                _str[i + pos] = c;
            }
            _size += n;
            return *this;
        }

        string& erase(size_t pos = 0, size_t len = npos)
        {
            assert(pos < _size);
            if(len == npos || pos + len >= _size)
            {
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                size_t end = pos + len;
                while(end <= _size)
                {
                    _str[pos++] = _str[end++];
                }
                _size -= len;
            }
            return *this;
        }

        size_t find(const char c, size_t pos = 0) const
        {
            assert(pos < _size);
            for (size_t i = pos; i < _size; i++)
            {
                if(_str[i] == c)
                {
                    return i;
                }
            }
            return npos;
        }

        size_t find(const char* s, size_t pos = 0) const
        {
            assert(pos < _size);
            const char* ptr = strstr(_str, s);
            if(ptr)
            {
                return ptr - _str;
            }
            else
            {
                return npos;
            }
        }

        string substr (size_t pos = 0, size_t len = npos) const
        {
            assert(pos < _size);
            string ret;
            size_t end;
            if(pos + len > _size || len == npos)
            {
                end = _size;
            }
            else
            {
                end = pos + len;
            }
            for (size_t i = pos; i < end; i++)
            {
                ret.push_back(_str[i]);
            }
            return ret;
        }


    };
    size_t string::npos = -1;

    // 流
    ostream& operator<<(ostream& out, const string& s)
    {
        for(auto ch : s)
        {
            out << ch;
        }
        return out;
    }

    istream& operator>>(istream& in, string& s)
    {
        // in默认不读取空格和换行
        //in >> ch;
        char ch = in.get();
        // 清除缓冲区的空格和换行
        while(ch == ' ' || ch == '\n')
        {
            ch = in.get();
        }
        // 先清空s中的数据
        s.clear();
        // 扩容空间优化，先开有限空间重复利用，避免多次扩容
        char buff[128];
        int i = 0;
        while(ch != ' ' && ch != '\n')
        {
            buff[i++] = ch;
            // buff满了就放到s中，置0重新开始
            if(i == 127)
            {
                buff[i] = '\0';
                s += buff;
                i = 0;
            }
            //in >> ch;
            ch = in.get();
        }
        if(i != 0)
        {
            buff[i] = '\0';
            s += buff;
        }
        return in;
    }
};

