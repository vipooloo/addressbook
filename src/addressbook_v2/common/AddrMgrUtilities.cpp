#include "AddrMgrUtilities.h"
#include <algorithm>
#include <cerrno>
#include <chrono>  // std::chrono::system_clock
#include <climits>
#include <cstdlib>
#include <ctime>    // std::time_t, localtime_r
#include <iomanip>  // std::put_time
#include <iostream>
#include <sstream>  // std::stringstream
#include <string>

std::vector<uint32_t> AddrMgrUtilities::GetSortedUniqueRids(const std::vector<uint32_t>& rids)
{
    std::vector<uint32_t> unique_rids = rids;
    std::sort(unique_rids.begin(), unique_rids.end());
    std::vector<uint32_t>::iterator last = std::unique(unique_rids.begin(), unique_rids.end());
    unique_rids.erase(last, unique_rids.end());
    return unique_rids;
}

std::string AddrMgrUtilities::JoinIds(const std::vector<uint32_t>& rids)
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

std::vector<std::string> AddrMgrUtilities::Split(const std::string& s, const std::string& delimiter)
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

uint32_t AddrMgrUtilities::ConvertToNumber(const std::string& s)
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

std::vector<uint32_t> AddrMgrUtilities::ConvertToNumbers(const std::vector<std::string>& s)
{
    std::vector<uint32_t> numbers;
    std::transform(s.begin(), s.end(), std::back_inserter(numbers), &AddrMgrUtilities::ConvertToNumber);
    return numbers;
}

std::string AddrMgrUtilities::GenerateTimestampedFileName(const std::string& prefix, const std::string& suffix)
{
    // 1. 获取当前系统时间
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // 2. 使用 localtime_r (Linux/POSIX 专用)
    // 区别：我们需要自己定义一个 tm 结构体变量，而不是用指针接收系统的静态变量
    std::tm now_tm = {};  // 初始化为空

    // localtime_r 的第一个参数是时间戳指针，第二个参数是接收结果的 buffer 指针
    // 它返回值也是这个 buffer 指针，如果失败返回 NULL
    if (localtime_r(&now_c, &now_tm) == nullptr)
    {
        // 极端情况下的错误处理（通常不会发生），可以返回一个默认值或空字符串
        return prefix + "_error" + suffix;
    }

    // 3. 格式化拼接
    std::stringstream ss;
    ss << prefix;

    // 智能补全下划线：如果前缀不为空且没自带下划线，就补一个
    if (!prefix.empty() && prefix.back() != '_')
    {
        ss << "_";
    }

    // 4. 使用 put_time 格式化
    // 格式：YYYY.MM.DD_HHMMSS (例如 2026.01.05_143005)
    ss << std::put_time(&now_tm, "%Y.%m.%d_%H%M%S");

    ss << suffix;

    return ss.str();
}

std::string AddrMgrUtilities::ReplaceFirst(const std::string& str, const std::string& replacement)
{
    return AddrMgrUtilities::ReplaceFirst(str, "%s", replacement);
}

std::string AddrMgrUtilities::ReplaceFirst(const std::string& str, const std::string& needle, const std::string& replacement)
{
    std::string ret = str;
    // 1. 查找第一个出现的位置
    size_t pos = ret.find(needle);

    // 2. 如果找到了，就替换它
    if (pos != std::string::npos)
    {
        ret.replace(pos, needle.length(), replacement);
    }
    return ret;
}