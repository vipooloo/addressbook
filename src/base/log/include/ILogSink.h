#ifndef ILOGSINK_H
#define ILOGSINK_H

#include "LogLevel.h"
#include <string>

class ILogSink
{
  public:
    virtual ~ILogSink() = default;
    virtual void log(const char* modulename, const char* filename, int32_t line, const char* funcname, LOG_LEVEL lvl, const std::string& msg) = 0;
};

#endif  // ILOGSINK_H