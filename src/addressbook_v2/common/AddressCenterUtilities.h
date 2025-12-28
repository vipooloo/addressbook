#ifndef ADDRESSCENTERUTILITIES_H
#define ADDRESSCENTERUTILITIES_H

#include <cstdint>
#include <string>
#include <vector>

class AddressCenterUtilities
{
  public:
    static std::vector<uint32_t> GetSortedUniqueRids(const std::vector<uint32_t>& rids);
    static std::string JoinIds(const std::vector<uint32_t>& rids);
};

#endif  // ADDRESSCENTERUTILITIES_H