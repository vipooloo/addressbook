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

std::string AddressCenterUtilities::JoinIds(const std::vector<uint32_t>& rids)
{
    std::string result;
    if (!rids.empty())
    {
        // 预估大小，避免多次内存分配
        result.reserve(rids.size() * 10);
        result += std::to_string(rids[0]);
        for (size_t i = 1; i < rids.size(); ++i)
        {
            result += ",";
            result += std::to_string(rids[i]);
        }
    }
    return result;
}