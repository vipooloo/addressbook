#include "StringUtility.h"
#include <algorithm>
#include <ctype.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace BW {
namespace Util {
bool StringUtility::string_compare(const std::string& a, const std::string& b, bool case_insensitive)
{
    if (!case_insensitive)
    {
        return a == b;
    }

    if (a.size() != b.size())
    {
        return false;
    }

    return std::equal(a.begin(), a.end(), b.begin(), [](char c1, char c2) {
        return std::tolower(static_cast<unsigned char>(c1)) ==
               std::tolower(static_cast<unsigned char>(c2));
    });
}

std::string StringUtility::stiring_to_lower(const std::string& str)
{
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return lower_str;
}

std::string StringUtility::stiring_to_upper(const std::string& str)
{
    std::string upper_str = str;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return upper_str;
}

std::string StringUtility::rawDataToHexStr(const void* data_ptr, uint16_t data_len, bool uppercase, const std::string& delimiter)
{
    if ((data_ptr == NULL) || (data_len == 0))
        return std::string();

    using namespace std;
    ostringstream oss;
    oss << hex << setfill('0');
    if (uppercase)
    {
        oss << std::uppercase;
    }

    const uint8_t* ptr = static_cast<const uint8_t*>(data_ptr);
    for (uint16_t i = 0; i < data_len; ++i)
    {
        oss << setw(2) << static_cast<uint32_t>(ptr[i]);
        if (i < (data_len - 1))
            oss << delimiter;
    }

    return oss.str();
}

}  // namespace Util
}  // namespace BW