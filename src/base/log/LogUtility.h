#ifndef LOGUTILITY_H
#define LOGUTILITY_H

#include <stdint.h>
#include <string>

constexpr uint32_t MAX_LOG_NAME_LEN = 8;

std::string formatString(const std::string& input)
{
    std::string modulename = input;
    if (modulename.empty())
    {
        modulename = "UNKNOW";
    }

    std::string result;

    if (modulename.length() > MAX_LOG_NAME_LEN)
    {
        result = modulename.substr(0, MAX_LOG_NAME_LEN);  // 截取前6位
    }
    else
    {
        // 前面用空格补齐到6位
        result = std::string(MAX_LOG_NAME_LEN - modulename.length(), ' ') + modulename;
    }

    return "[" + result + "]" + " ";  // 添加方括号
}

#endif  // LOGUTILITY_H