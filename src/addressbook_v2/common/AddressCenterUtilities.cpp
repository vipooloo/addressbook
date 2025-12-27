#include "AddressCenterUtilities.h"
#include <algorithm>

std::vector<uint32_t> AddressCenterUtilities::GetSortedUniqueRids(const std::vector<uint32_t>& rids)
{
    std::vector<uint32_t> unique_rids = rids;
    std::sort(unique_rids.begin(), unique_rids.end());
    std::vector<uint32_t>::iterator last = std::unique(unique_rids.begin(), unique_rids.end());
    unique_rids.erase(last, unique_rids.end());
    return unique_rids;
}

std::pair<std::vector<uint32_t>, std::vector<uint32_t>>
AddressCenterUtilities::CalculateIdDiff(const std::vector<uint32_t>& new_ids, const std::vector<uint32_t>& old_ids)
{
    // 1. 必须对输入进行排序，std::set_difference 依赖有序序列
    //    由于传入的是 const 引用，我们需要拷贝一份进行排序
    std::vector<uint32_t> sorted_new = new_ids;
    std::vector<uint32_t> sorted_old = old_ids;
    std::sort(sorted_new.begin(), sorted_new.end());
    std::sort(sorted_old.begin(), sorted_old.end());

    std::vector<uint32_t> to_add;     // 需要新增的部分 (New - Old)
    std::vector<uint32_t> to_remove;  // 需要删除的部分 (Old - New)

    // 预分配内存以优化性能（估算大小）
    to_add.reserve(sorted_new.size());
    to_remove.reserve(sorted_old.size());

    // 2. 计算需新增部分: New - Old
    std::set_difference(
        sorted_new.begin(),
        sorted_new.end(),
        sorted_old.begin(),
        sorted_old.end(),
        std::back_inserter(to_add));

    // 3. 计算需删除部分: Old - New
    std::set_difference(
        sorted_old.begin(),
        sorted_old.end(),
        sorted_new.begin(),
        sorted_new.end(),
        std::back_inserter(to_remove));

    return {to_add, to_remove};
}