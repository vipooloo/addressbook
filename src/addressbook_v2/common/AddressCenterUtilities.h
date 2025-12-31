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
    static std::vector<std::string> Split(const std::string& s, const std::string& delimiter);
    static uint32_t ConvertToNumber(const std::string& s);
    static std::vector<uint32_t> ConvertToNumbers(const std::vector<std::string>& s);
};

#endif  // ADDRESSCENTERUTILITIES_H