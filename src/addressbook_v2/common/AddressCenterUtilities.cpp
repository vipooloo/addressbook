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
