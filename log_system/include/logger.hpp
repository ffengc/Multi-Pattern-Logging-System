/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_LOGGER__
#define __YUFC_LOGGER__

#include "format.hpp"
#include "level.hpp"
#include "sink.hpp"
#include "util.hpp"
#include <atomic>
#include <mutex>
#include <stdarg.h>

namespace ffengc_log {
class logger {
protected:
    std::string __logger_name;
    std::atomic<logLevel::value> __limit_level; // 因为需要多次访问，所以用原子类型
    formatter::ptr __formatter; // 格式化器
    std::vector<logSink::ptr> __sinks; // 落地方向，可能有个
    std::mutex __mtx; //
public:
    using ptr = std::shared_ptr<logger>;
    logger(const std::string& logger_name,
        logLevel::value level,
        formatter::ptr& ft,
        const std::vector<logSink::ptr>& sinks)
        : __logger_name(logger_name)
        , __limit_level(level)
        , __formatter(ft)
        , __sinks(sinks.begin(), sinks.end()) { }
    void debug(const std::string& file, size_t line, const std::string& fmt, ...) {
        auto function_log_level = logLevel::value::DEBUG;
        // 通过传入的参数，构造一个日志消息对象
        // 1. 判断当前日志是否达到了输出等级
        if (function_log_level < __limit_level)
            return;
        // 2. 对不定参消息组织成一个字符串
        va_list ap;
        va_start(ap, fmt);
        char* res;
        int ret = vasprintf(&res, fmt.c_str(), ap);
        if (ret == -1) {
            std::cerr << "vasprintf failed!" << std::endl;
            return;
        }
        va_end(ap);
        // 3. 构造logMessage对象
        logMessage msg(function_log_level, line, file, __logger_name, res);
        // 4. 通过格式化工具对 logMessage 进行格式化，得到格式化后的日志字符串
        std::stringstream ss;
        __formatter->format(ss, msg);
        // 5. 落地
        log(ss.str().c_str(), ss.str().size());
        free(res); // 不要忘记了
    }
    void info(const std::string& file, size_t line, const std::string& fmt, ...) {
        auto function_log_level = logLevel::value::INFO;
        // 通过传入的参数，构造一个日志消息对象
        // 1. 判断当前日志是否达到了输出等级
        if (function_log_level < __limit_level)
            return;
        // 2. 对不定参消息组织成一个字符串
        va_list ap;
        va_start(ap, fmt);
        char* res;
        int ret = vasprintf(&res, fmt.c_str(), ap);
        if (ret == -1) {
            std::cerr << "vasprintf failed!" << std::endl;
            return;
        }
        va_end(ap);
        // 3. 构造logMessage对象
        logMessage msg(function_log_level, line, file, __logger_name, res);
        // 4. 通过格式化工具对 logMessage 进行格式化，得到格式化后的日志字符串
        std::stringstream ss;
        __formatter->format(ss, msg);
        // 5. 落地
        log(ss.str().c_str(), ss.str().size());
        free(res); // 不要忘记了
    }
    void warning(const std::string& file, size_t line, const std::string& fmt, ...) {
        auto function_log_level = logLevel::value::WARNING;
        // 通过传入的参数，构造一个日志消息对象
        // 1. 判断当前日志是否达到了输出等级
        if (function_log_level < __limit_level)
            return;
        // 2. 对不定参消息组织成一个字符串
        va_list ap;
        va_start(ap, fmt);
        char* res;
        int ret = vasprintf(&res, fmt.c_str(), ap);
        if (ret == -1) {
            std::cerr << "vasprintf failed!" << std::endl;
            return;
        }
        va_end(ap);
        // 3. 构造logMessage对象
        logMessage msg(function_log_level, line, file, __logger_name, res);
        // 4. 通过格式化工具对 logMessage 进行格式化，得到格式化后的日志字符串
        std::stringstream ss;
        __formatter->format(ss, msg);
        // 5. 落地
        log(ss.str().c_str(), ss.str().size());
        free(res); // 不要忘记了
    }
    void error(const std::string& file, size_t line, const std::string& fmt, ...) {
        auto function_log_level = logLevel::value::ERROR;
        // 通过传入的参数，构造一个日志消息对象
        // 1. 判断当前日志是否达到了输出等级
        if (function_log_level < __limit_level)
            return;
        // 2. 对不定参消息组织成一个字符串
        va_list ap;
        va_start(ap, fmt);
        char* res;
        int ret = vasprintf(&res, fmt.c_str(), ap);
        if (ret == -1) {
            std::cerr << "vasprintf failed!" << std::endl;
            return;
        }
        va_end(ap);
        // 3. 构造logMessage对象
        logMessage msg(function_log_level, line, file, __logger_name, res);
        // 4. 通过格式化工具对 logMessage 进行格式化，得到格式化后的日志字符串
        std::stringstream ss;
        __formatter->format(ss, msg);
        // 5. 落地
        log(ss.str().c_str(), ss.str().size());
        free(res); // 不要忘记了
    }
    void fatal(const std::string& file, size_t line, const std::string& fmt, ...) {
        auto function_log_level = logLevel::value::FATAL;
        // 通过传入的参数，构造一个日志消息对象
        // 1. 判断当前日志是否达到了输出等级
        if (function_log_level < __limit_level)
            return;
        // 2. 对不定参消息组织成一个字符串
        va_list ap;
        va_start(ap, fmt);
        char* res;
        int ret = vasprintf(&res, fmt.c_str(), ap);
        if (ret == -1) {
            std::cerr << "vasprintf failed!" << std::endl;
            return;
        }
        va_end(ap);
        // 3. 构造logMessage对象
        logMessage msg(function_log_level, line, file, __logger_name, res);
        // 4. 通过格式化工具对 logMessage 进行格式化，得到格式化后的日志字符串
        std::stringstream ss;
        __formatter->format(ss, msg);
        // 5. 落地
        log(ss.str().c_str(), ss.str().size());
        free(res); // 不要忘记了
    } //
protected:
    virtual void log(const char* data, size_t len) = 0; // 实际的落地由它来完成
};
/* 同步日志器是将日志直接通过落地模块句柄进行落地 */
class syncLogger : public logger {
private:
    void log(const char* data, size_t len) {
        std::unique_lock<std::mutex> lock(__mtx);
        if (__sinks.empty())
            return;
        for (const auto& e : __sinks)
            e->log(data, len);
    } //
public:
    syncLogger(const std::string& logger_name,
        logLevel::value level,
        formatter::ptr& ft,
        const std::vector<logSink::ptr>& sinks)
        : logger(logger_name, level, ft, sinks) { }
};
// 1. 抽象一个建造者类
// 2. 派生一个具体的建造者类
//  2.1 局部日志器建造者类
//  2.2 全局的日志器建造者类
// tips: 因为我们的logger对参数顺序没有要求，因此在这里省略指挥者类
} // namespace ffengc_log

#endif