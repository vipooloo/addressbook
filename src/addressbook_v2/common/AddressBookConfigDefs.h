#ifndef ADDRESSBOOKCONFIGDEFS_H
#define ADDRESSBOOKCONFIGDEFS_H

#include <cstdint>

static constexpr uint32_t kMaxMailCount = 100;        ///< 最多支持多少个邮件
static constexpr uint32_t kMaxGroupCount = 100;       ///< 最多支持多少个邮件组
static constexpr uint32_t kMaxGroupsPerEmail = 100;   ///< 单个邮件最多可归属的邮件组数量
static constexpr uint32_t kMaxEmailsPerGroup = 100;  ///< 单个邮件组最多可包含的邮件数量

#endif  // ADDRESSBOOKCONFIGDEFS_H