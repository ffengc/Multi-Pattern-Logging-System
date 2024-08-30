/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_LEVEL__
#define __YUFC_LEVEL__

namespace ffengc_log {
class logLevel {
public:
    enum class value {
        UNKNOW = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        OFF
    };
    static const char* toString(logLevel::value level) {
        switch (level) {
        case logLevel::value::DEBUG:
            return "DEBUG";
        case logLevel::value::INFO:
            return "INFO";
        case logLevel::value::WARNING:
            return "WARNING";
        case logLevel::value::ERROR:
            return "ERROR";
        case logLevel::value::FATAL:
            return "FATAL";
        case logLevel::value::OFF:
            return "OFF";
        default:
            return "UNKNOW";
        }
        return "UNKNOW";
    }
};
} // namespace ffengc_log

#endif