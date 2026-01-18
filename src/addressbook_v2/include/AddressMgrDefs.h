#ifndef ADDRESSMGRDEFS_H
#define ADDRESSMGRDEFS_H

#include <cstdint>
#include <functional>
#include <string>

using ImportExportCallback = std::function<void(
    const std::string& file_name,  ///< 文件名字
    bool result                    ///< 是否成功
    )>;

enum class ResultCode : uint8_t
{
    kSuccess = 0,     // 成功
    kInvalidParam,    // 参数无效
    kNotFound,        // 数据不存在
    kAlreadyExist,    // 数据重复
    kDbError,         // 数据库错误
    kExceedMaxCount,  // 超出最大数量限制（新增）
    kNotable,         // 不可用
};

#endif  // ADDRESSMGRDEFS_H