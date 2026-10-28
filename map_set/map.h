#pragma once
#include "RBtree.h"
#include <utility>

namespace cai
{
    template<typename K, typename V>
    class map
    {
        // 适配器类，提供一个函数对象，返回 pair 的第一个元素（key）
        struct MapKeyOfT
        {
            const K& operator()(const std::pair<K, V>& kv) const
            {
                return kv.first;
            }
        };

    public:
        // map的迭代器可以是iterator，因为map中的元素是不可修改的，但可以通过迭代器修改value
        typedef typename RBtree<K, std::pair<const K, V>, MapKeyOfT>::iterator iterator;
        typedef typename RBtree<K, std::pair<const K, V>, MapKeyOfT>::const_iterator const_iterator;
        iterator begin()
        {
            return _t.begin();
        }

        iterator end()
        {
            return _t.end();
        }

        const_iterator begin() const
        {
            return _t.begin();
        }

        const_iterator end() const
        {
            return _t.end();
        }
        
        bool insert(const std::pair<K, V>& kv)
        {
            return _t.insert(kv);
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
        RBtree<K, std::pair<const K, V>, MapKeyOfT> _t;
    };
}