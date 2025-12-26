#ifndef ADDRCENTERLOG_H
#define ADDRCENTERLOG_H

#include "Log.h"

#define AB_LOG_I(fmt, ...) LOGP_INFO_M("AddrCenter", fmt, ##__VA_ARGS__)
#define AB_LOG_E(fmt, ...) LOGP_ERROR_M("AddrCenter", fmt, ##__VA_ARGS__)
#define AB_LOG_W(fmt, ...) LOGP_WARN_M("AddrCenter", fmt, ##__VA_ARGS__)

#endif  // ADDRCENTERLOG_H