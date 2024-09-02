/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __FFENGC_LOG__
#define __FFENGC_LOG__

#include "internal/logger.hpp"

namespace ffengc_log {
/* Get a logger */
logger::ptr getLogger(const std::string& name) { return loggerManager::getInstance().get(name); }
/* Get default logger */
logger::ptr rootLogger() { return loggerManager::getInstance().get_root(); }
// proxy
#define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define warning(fmt, ...) warning(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
// default output
#define DLOG_DEBUG(fmt, ...) ffengc_log::rootLogger()->debug(fmt, ##__VA_ARGS__);
#define DLOG_INFO(fmt, ...) ffengc_log::rootLogger()->info(fmt, ##__VA_ARGS__);
#define DLOG_WARNING(fmt, ...) ffengc_log::rootLogger()->warning(fmt, ##__VA_ARGS__);
#define DLOG_ERROR(fmt, ...) ffengc_log::rootLogger()->error(fmt, ##__VA_ARGS__);
#define DLOG_FATAL(fmt, ...) ffengc_log::rootLogger()->fatal(fmt, ##__VA_ARGS__);
// logger output
#define LOG_DEBUG(name, fmt, ...) ffengc_log::getLogger(name)->debug(fmt, ##__VA_ARGS__);
#define LOG_INFO(name, fmt, ...) ffengc_log::getLogger(name)->info(fmt, ##__VA_ARGS__);
#define LOG_WARNING(name, fmt, ...) ffengc_log::getLogger(name)->warning(fmt, ##__VA_ARGS__);
#define LOG_ERROR(name, fmt, ...) ffengc_log::getLogger(name)->error(fmt, ##__VA_ARGS__);
#define LOG_FATAL(name, fmt, ...) ffengc_log::getLogger(name)->fatal(fmt, ##__VA_ARGS__);
//
} // namespace ffengc_log

#endif