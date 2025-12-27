#ifndef ADDRESSCENTERUTILITIES_H
#define ADDRESSCENTERUTILITIES_H

#include <cstdint>
#include <vector>

class AddressCenterUtilities
{
  public:
    static std::vector<uint32_t> GetSortedUniqueRids(const std::vector<uint32_t>& rids);
    /**
     * @brief 计算两个 ID 列表的差异（计算新增和删除的部分）
     * @param new_ids 前端传入的最新 ID 列表
     * @param old_ids 数据库中已存在的旧 ID 列表
     * @return pair.first: 需要新增的 IDs (to_add)
     * pair.second: 需要删除的 IDs (to_remove)
     */
    static std::pair<std::vector<uint32_t>, std::vector<uint32_t>>
    CalculateIdDiff(const std::vector<uint32_t>& new_ids, const std::vector<uint32_t>& old_ids);
};

#endif  // ADDRESSCENTERUTILITIES_H