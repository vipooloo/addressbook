#ifndef ADDRCENTERDEFS_H
#define ADDRCENTERDEFS_H

#include <cstdint>
#include <functional>
#include <string>

using ImportExportCallback = std::function<void(
    const std::string& file_name,  ///< 导出文件名字
    bool result                    ///< 导出是否成功
    )>;

enum class ResultCode : uint8_t
{
    kSuccess = 0,         // 成功
    kInvalidParam = 1,    // 参数无效
    kNotFound = 2,        // 数据不存在
    kDuplicate = 3,       // 数据重复
    kDbError = 4,         // 数据库错误
    kGroupNotEmpty = 5,   // 邮件组非空（删除失败）
    kExceedMaxCount = 6,  // 超出最大数量限制（新增）
    kNotable = 7,         // 不可用
};

#endif  // ADDRCENTERDEFS_H