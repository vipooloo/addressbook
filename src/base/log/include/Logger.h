#ifndef LOGGER_H
#define LOGGER_H

#include "ILogSink.h"
#include <fmt/core.h>  // fmt formatting
#include <memory>
#include <string>

class Logger
{
  public:
    static void Init();
    static Logger& GetInstance();

    template<typename... Args>
    void log(const char* module, const char* filename, int line, const char* funcname, LOG_LEVEL lvl, const std::string& fmtStr, Args&&... args)
    {
        if (impl_)
        {
            impl_->log(module,    ///<
                       filename,  ///<
                       line,      ///<
                       funcname,  ///<
                       lvl,       ///<
                       fmt::format(fmtStr, std::forward<Args>(args)...));
        }
    }

  private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

  private:
    static std::shared_ptr<ILogSink> impl_;
};

#endif  // LOGGER_H