/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

// show how to use these logs

#include "log.h"

void init_logger() {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("this project");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger = builder->build();
}

void use_logger() {
    ffengc_log::logger::ptr logger = ffengc_log::loggerManager::getInstance().get("this project");
    std::string str = "log test from logManager_test";
    DLOG_DEBUG("hello world");
    LOG_FATAL("this project", "hello world");
}

int main() {
    init_logger();
    use_logger();
    return 0;
}