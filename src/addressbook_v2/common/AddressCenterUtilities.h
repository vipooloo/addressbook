#ifndef ADDRESSCENTERUTILITIES_H
#define ADDRESSCENTERUTILITIES_H

#include <cstdint>
#include <vector>

class AddressCenterUtilities
{
  public:
    static std::vector<uint32_t> GetSortedUniqueRids(const std::vector<uint32_t>& rids);
};

#endif  // ADDRESSCENTERUTILITIES_H