
#include "Logger.h"
#include "Log.h"
#include "SpdLogSink.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <array>
#include <cstdarg>
#include <string>

std::shared_ptr<ILogSink> Logger::impl_ = nullptr;

Logger::Logger()
{
    Logger::impl_ = std::make_shared<SpdLogSink>();
}

Logger::~Logger()
{
}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::Init()
{
    GetInstance();
}

constexpr uint32_t MAX_LOGLINELENGTH = 256;

void _LogPrintfFunc(const char* module_name, const char* file_name, int32_t line, const char* func_name, LOG_LEVEL lvl, const char* fmt, ...)
{
    va_list args{};
    va_start(args, fmt);
    char buffer[MAX_LOGLINELENGTH]{};
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[MAX_LOGLINELENGTH - 1] = '\0';
    std::string message = formatString(module_name) + buffer;
    spdlog::default_logger()->log(spdlog::source_loc{file_name, line, func_name}, static_cast<spdlog::level::level_enum>(lvl), message);
}
