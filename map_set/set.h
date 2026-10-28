#pragma once
#include "RBtree.h"

namespace cai
{
    template<typename K>
    class set
    {
        // 适配器类，提供一个函数对象，返回 key 本身
        struct SetKeyOfT
        {
            const K& operator()(const K& key) const
            {
                return key;
            }
        };

    public:
        // set的迭代器只能是const_iterator，因为set中的元素是不可修改的
        typedef typename RBtree<K, K, SetKeyOfT>::const_iterator iterator;
        typedef typename RBtree<K, K, SetKeyOfT>::const_iterator const_iterator;
        // iterator是const_iterator的别名，故只需一份const版本
        iterator begin() const
        {
            return _t.begin();
        }

        iterator end() const
        {
            return _t.end();
        }
        
        bool insert(const K& key)
        {
            return _t.insert(key);
        }

        bool find(const K& key) const
        {
            return _t.find(key) != nullptr;
        }

        std::size_t size() const
        {
            return _t.size();
        }

        bool isRBTree() const
        {
            return _t.isRBTree();
        }

    private:
        RBtree<K, K, SetKeyOfT> _t;
    };
}