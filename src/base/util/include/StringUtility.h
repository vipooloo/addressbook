#ifndef STRINGUTILITY_H
#define STRINGUTILITY_H

#include <string>

namespace BW {
namespace Util {

class StringUtility
{
  public:
    static bool string_compare(const std::string& a, const std::string& b, bool case_insensitive = false);

    static std::string stiring_to_lower(const std::string& str);

    static std::string stiring_to_upper(const std::string& str);

    static std::string rawDataToHexStr(const void* data_ptr, uint16_t data_len, bool uppercase, const std::string& delimiter);

  private:
};

}  // namespace Util
}  // namespace BW

#endif  // STRINGUTILITY_H