#ifndef ADDRMGRUTILITIES_H
#define ADDRMGRUTILITIES_H

#include <cstdint>
#include <string>
#include <vector>

class AddrMgrUtilities
{
  public:
    static std::vector<uint32_t> GetSortedUniqueRids(const std::vector<uint32_t>& rids);
    static std::string JoinIds(const std::vector<uint32_t>& rids);
    static std::vector<std::string> Split(const std::string& s, const std::string& delimiter);
    static uint32_t ConvertToNumber(const std::string& s);
    static std::vector<uint32_t> ConvertToNumbers(const std::vector<std::string>& s);
    /**
     * @brief 生成带时间戳的文件名 (Linux/POSIX 线程安全版)
     * @param prefix 文件前缀 (例如 "emails")
     * @param suffix 文件后缀 (例如 ".csv")
     * @return 格式化后的文件名 (例如 "emails_2026.01.05_143005.csv")
     */
    static std::string GenerateTimestampedFileName(const std::string& prefix, const std::string& suffix);
};

#endif  // ADDRMGRUTILITIES_H