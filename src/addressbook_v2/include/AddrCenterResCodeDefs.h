#ifndef ADDRCENTERRESCODEDEFS_H
#define ADDRCENTERRESCODEDEFS_H

#include <cstdint>

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

#endif  // ADDRCENTERRESCODEDEFS_H