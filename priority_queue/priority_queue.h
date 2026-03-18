#pragma once
#include <vector>

namespace cai
{
    template<class T, class Container = std::vector<T>, class Compare = std::less<T>>
    class priority_queue
    {
        private:
            Container _con;
            Compare _comp;

            void adjust_down(int root)
            {
                size_t parent = root;
                size_t child = parent * 2 + 1; // 默认先看左孩子
            
                while (child < _con.size())
                {
                    // 选出左右孩子中较大的那个
                    // 如果 左孩子 < 右孩子，就选右孩子
                    if (child + 1 < _con.size() && _comp(_con[child], _con[child + 1]))
                    {
                       ++child;
                    }

                    // 如果 父亲 < 孩子，就向下交换
                    if (_comp(_con[parent], _con[child]))
                    {
                        std::swap(_con[parent], _con[child]);
                        parent = child;
                        child = parent * 2 + 1;
                    }
                    else
                    {
                        break; // 父亲已经足够大，调整结束
                    }
                }
            }

            void adjust_up(int child)
            {
                int parent = (child - 1) / 2;
                while (child > 0)
                {
                    // 如果 父亲 < 孩子，则向上交换
                    if (_comp(_con[parent], _con[child])) 
                    {
                        std::swap(_con[child], _con[parent]);
                        child = parent;
                        parent = (child - 1) / 2;
                    }
                    else
                    {
                        break; // 父亲已经不小于孩子了，调整结束
                    }
                }
            }

        public:
            priority_queue()
            {}
            
            // 默认建立大根堆
            template<class Inputiterator>
            priority_queue(Inputiterator first, Inputiterator last)
            {
                for(auto it = first; it != last; ++it)
                {
                    _con.push_back(*it);
                }

                // 构建堆
                for(int i = _con.size() / 2 - 1; i >= 0; --i)
                {
                    adjust_down(i);
                }
            }

            const T& top() const
            {
                return _con[0];
            }

            void pop()
            {
                std::swap(_con[0], _con[_con.size() - 1]);
                _con.pop_back();
                adjust_down(0);
            } 

            void push(const T& value)
            {
                _con.push_back(value);
                adjust_up(_con.size() - 1);
            }

            bool empty() const
            {
                return _con.empty();
            }

            size_t size() const
            {
                return _con.size();
            }
        
    };
}