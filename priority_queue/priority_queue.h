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
                int parent = root;
                int child = 2 * parent + 1;
                while(child < _con.size())
                {
                    // 选出左右孩子中较小的一个
                    if(child + 1 < _con.size() && _comp(_con[child], _con[child + 1]))
                    {
                        ++child;
                    }
                    // 如果父节点已经比孩子节点小了，就不需要调整了
                    if(!_comp(_con[parent], _con[child]))
                    {
                        break;
                    }
                    std::swap(_con[parent], _con[child]);
                    parent = child;
                }
            
            }

            void adjust_up(int child)
            {
                int parent = (child - 1) / 2;
                while(child > 0 && _comp(_con[child], _con[parent]))
                {
                    std::swap(_con[child], _con[parent]);
                    child = parent;
                }
            }
        public:
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