#ifndef ADDRESSRESULTCODEDEFS_H
#define ADDRESSRESULTCODEDEFS_H

enum class ErrorCode
{
    kSuccess = 0,         // 成功
    kInvalidParam = 1,    // 参数无效
    kNotFound = 2,        // 数据不存在
    kDuplicate = 3,       // 数据重复
    kDbError = 4,         // 数据库错误
    kGroupNotEmpty = 5,   // 邮件组非空（删除失败）
    kExceedMaxCount = 6,  // 超出最大数量限制（新增）
};

#endif  // ADDRESSRESULTCODEDEFS_H