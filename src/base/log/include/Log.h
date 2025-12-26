#ifndef LOG_H
#define LOG_H

#include "LogLevel.h"
#include "Logger.h"

#define LOG_INIT() Logger::Init()

// —— 默认模块为 "" 的日志宏 ——
#define LOGX_FATAL(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_F, fmt, ##__VA_ARGS__)
#define LOGX_ERROR(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_E, fmt, ##__VA_ARGS__)
#define LOGX_WARN(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_W, fmt, ##__VA_ARGS__)
#define LOGX_INFO(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_I, fmt, ##__VA_ARGS__)
#define LOGX_DEBUG(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_D, fmt, ##__VA_ARGS__)
#define LOGX_TRACE(fmt, ...) \
    Logger::GetInstance().log("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_T, fmt, ##__VA_ARGS__)

// —— 带模块名的日志宏 ——
#define LOGX_FATAL_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_F, fmt, ##__VA_ARGS__)
#define LOGX_ERROR_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_E, fmt, ##__VA_ARGS__)
#define LOGX_WARN_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_W, fmt, ##__VA_ARGS__)
#define LOGX_INFO_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_I, fmt, ##__VA_ARGS__)
#define LOGX_DEBUG_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_D, fmt, ##__VA_ARGS__)
#define LOGX_TRACE_M(mod, fmt, ...) \
    Logger::GetInstance().log(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_T, fmt, ##__VA_ARGS__)

extern "C" {
void _LogPrintfFunc(const char* module_name, const char* file_name, int32_t line, const char* func_name, LOG_LEVEL lvl, const char* fmt, ...);
}

#define LOGP_FATAL(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_F, fmt, ##__VA_ARGS__)
#define LOGP_ERROR(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_E, fmt, ##__VA_ARGS__)
#define LOGP_WARN(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_W, fmt, ##__VA_ARGS__)
#define LOGP_INFO(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_I, fmt, ##__VA_ARGS__)
#define LOGP_DEBUG(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_D, fmt, ##__VA_ARGS__)
#define LOGP_TRACE(fmt, ...) \
    _LogPrintfFunc("", __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_T, fmt, ##__VA_ARGS__)

#define LOGP_FATAL_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_F, fmt, ##__VA_ARGS__)
#define LOGP_ERROR_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_E, fmt, ##__VA_ARGS__)
#define LOGP_WARN_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_W, fmt, ##__VA_ARGS__)
#define LOGP_INFO_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_I, fmt, ##__VA_ARGS__)
#define LOGP_DEBUG_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_D, fmt, ##__VA_ARGS__)
#define LOGP_TRACE_M(mod, fmt, ...) \
    _LogPrintfFunc(mod, __FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_T, fmt, ##__VA_ARGS__)

#endif  // LOG_H