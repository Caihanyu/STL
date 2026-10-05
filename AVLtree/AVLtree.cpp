#include "AVLtree.h"

#include <vector>
#include <numeric>
#include <random>
#include <string>
#include <cmath>


int main()
{
    // ---------- 1. 定点插入：每插一个都校验 _bf 是否正确 ----------
    AVLTree<int, std::string> t;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65};
    for(int key : keys)
    {
        t.insert(std::make_pair(key, std::to_string(key)));
        if(!t.isBalance())
        {
            std::cout << "插入 " << key << " 之后 _bf 不对了！" << std::endl;
            return 1;
        }
        std::cout << "插入 " << key << " 后 height = " << t.height() << std::endl;
    }
    t.InOrder();

    // ---------- 2. 重复键不允许插入 ----------
    bool again = t.insert(std::pair<int, std::string>(50, "dup"));
    std::cout << "重复插入 50：" << (again ? "成功了(不对!)" : "被拒绝") << std::endl;

    // ---------- 3. 随机顺序插入 1000 个数据，检查是否始终平衡 ----------
    std::vector<int> nums(1000);
    std::iota(nums.begin(), nums.end(), 1);
    std::mt19937 gen(2026);
    std::shuffle(nums.begin(), nums.end(), gen);

    AVLTree<int, int> big;
    for(int x : nums)
    {
        big.insert(std::make_pair(x, x));
        if(!big.isBalance())
        {
            std::cout << "插入 " << x << " 之后 _bf 不对了！" << std::endl;
            return 1;
        }
    }
    int limit = (int)(1.45 * std::log2(1000.0) + 1);
    std::cout << "1000 个数据插入完成，height = " << big.height()
              << "（AVL 理论上限约 " << limit << "）" << std::endl;
    if(big.height() > limit)
    {
        std::cout << "树太高了，平衡没做好！" << std::endl;
        return 1;
    }

    std::cout << "全部通过" << std::endl;
    return 0;
}
