#include "AddressCenterUtilities.h"
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cstdlib>

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

std::vector<std::string> AddressCenterUtilities::Split(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> tokens;

    // 1. 只有当字符串不为空时才处理，否则直接返回空的 tokens
    if (!s.empty())
    {
        // 2. 只有分隔符不为空时才进行分割，防止死循环
        if (!delimiter.empty())
        {
            // 性能优化：简单的预估空间，避免频繁的内存重分配
            // 假设平均每段长度约为 8 字节，+1 避免 0
            tokens.reserve(s.length() / 8 + 1);

            size_t start = 0;
            size_t pos = 0;

            while ((pos = s.find(delimiter, start)) != std::string::npos)
            {
                tokens.push_back(s.substr(start, pos - start));
                start = pos + delimiter.length();
            }
            // 压入最后一个分隔符之后的内容
            tokens.push_back(s.substr(start));
        }
        else
        {
            // 如果分隔符为空，无法分割，将原字符串作为一个整体放入
            tokens.push_back(s);
        }
    }

    return tokens;
}

uint32_t AddressCenterUtilities::ConvertToNumber(const std::string& s)
{
    const char* begin = s.c_str();
    char* end = nullptr;

    errno = 0;
    unsigned long x = std::strtoul(begin, &end, 10);

    if (errno != 0 ||
        end == begin ||
        *end != '\0' ||
        x > UINT32_MAX)
    {
        return 0;
    }

    return static_cast<uint32_t>(x);
}

std::vector<uint32_t> AddressCenterUtilities::ConvertToNumbers(const std::vector<std::string>& s)
{
    std::vector<uint32_t> numbers;
    std::transform(s.begin(), s.end(), std::back_inserter(numbers), &AddressCenterUtilities::ConvertToNumber);
    return numbers;
}