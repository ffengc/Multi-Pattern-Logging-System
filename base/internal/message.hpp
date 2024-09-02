/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */


#ifndef __YUFC_MESSAGE__
#define __YUFC_MESSAGE__

#include "level.hpp"
#include "util.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace ffengc_log {
struct logMessage {
    time_t __ctime; // 日志产生的时间戳
    logLevel::value __level; // 日志等级
    size_t __line; // 行号
    std::thread::id __tid; // 线程id
    std::string __file; // 文件名
    std::string __logger; // 日志器名称
    std::string __payload; // 日志主体
    logMessage(const logLevel::value& level,
        const size_t& line,
        const std::string& file,
        const std::string& logger,
        const std::string& message)
        : __ctime(util::Date::now())
        , __level(level)
        , __line(line)
        , __tid(std::this_thread::get_id())
        , __file(file)
        , __logger(logger)
        , __payload(message) { }
};
} // namespace ffengc_log

#endif