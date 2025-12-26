#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdint.h>
#include <string>

namespace BW {
//! 获取当前的时间戳，精确到秒
uint32_t GetCurrentSecondsFrom1970();

//! 获取当前的时间戳，精确到毫秒
uint64_t GetCurrentMillisecondsFrom1970();

//! 获取当前的时间戳，精确到微秒
uint64_t GetCurrentMicrosecondsFrom1970();

//! 获取指定时间戳的0时区时间字串
std::string GetUtcTimeString();
std::string GetUtcTimeString(uint32_t utc_sec);

//! 获取指定时间戳的本地时间字串
std::string GetLocalTimeString(uint32_t utc_sec);
}  // namespace BW

#endif  // TIMESTAMP_H
