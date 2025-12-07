#ifndef SPDLOGSINK_H
#define SPDLOGSINK_H

#include "LogUtility.h"
#include "Logger.h"
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

class SpdLogSink : public ILogSink
{
  public:
    SpdLogSink()
    {
        std::string log_file_name = "BW.log";
        uint32_t log_max_size = 1024 * 1024 * 10;
        uint32_t log_max_files = 2;
        uint32_t log_flush_every = 1;
        std::string out_file_pattern = "%^[%L][%Y-%m-%d %H:%M:%S.%e][t:%t]%$ %v [%s:%#]";
        std::string out_cons_pattern = "%^[%L][%Y-%m-%d %H:%M:%S.%e][t:%t]%$ %v [%s:%#]";

        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file_name, log_max_size, log_max_files);
        file_sink->set_level(spdlog::level::debug);
        file_sink->set_pattern(out_file_pattern);

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);
        console_sink->set_pattern(out_cons_pattern);

        auto logger = std::make_shared<spdlog::logger>("logger", spdlog::sinks_init_list{file_sink, console_sink});
        logger->set_level(spdlog::level::debug);
        spdlog::set_default_logger(logger);
        spdlog::flush_every(std::chrono::seconds(log_flush_every));
    }

    ~SpdLogSink() override
    {
        //spdlog::shutdown();
    }

    void log(const char* modulename, const char* filename, int32_t line, const char* funcname, LOG_LEVEL lvl, const std::string& msg) override
    {
        auto loger_sptr = spdlog::default_logger();
        if (loger_sptr)
        {
            loger_sptr->log({filename, line, funcname},                   ///<
                            static_cast<spdlog::level::level_enum>(lvl),  ///<
                            "{}",                                         ///<
                            formatString(modulename) + msg                ///<
            );
        }
    }
};

#endif  // SPDLOGSINK_H