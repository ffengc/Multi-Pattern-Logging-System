/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#include "extension/extension_rollSinkByTime.hpp"
#include "format.hpp"
#include "level.hpp"
#include "logger.hpp"
#include "message.hpp"
#include "sink.hpp"
#include "util.hpp"
#include <gtest/gtest.h>

class all_test : public testing::Environment {
public:
    virtual void SetUp() override { }
    virtual void TearDown() override { }
};
TEST(all_test, util_test) {
    std::cout << ffengc_log::util::Date::now() << std::endl;
    std::string path_name = "./abc/bcd/a.txt";
    ffengc_log::util::File::createDirectory(ffengc_log::util::File::path(path_name));
}
TEST(all_test, level_test) {
    ASSERT_EQ(ffengc_log::logLevel::toString(ffengc_log::logLevel::value::DEBUG), "DEBUG");
    ASSERT_EQ(ffengc_log::logLevel::toString(ffengc_log::logLevel::value::UNKNOW), "UNKNOW");
}
TEST(all_test, format_test) {
    ffengc_log::logMessage msg(ffengc_log::logLevel::value::INFO,
        53,
        "main.c",
        "root",
        "fmt test...");
    ffengc_log::formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%j");
    std::string str = fmt.format(msg);
    std::cout << str << std::endl;
}
TEST(all_test, sink_basic_test) {
#define __test_extension_roll_sink_by_time true
    ffengc_log::logMessage msg(ffengc_log::logLevel::value::INFO,
        53,
        "main.c",
        "root",
        "fmt test...");
    ffengc_log::formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n");
    std::string str = fmt.format(msg);
#if !__test_extension_roll_sink_by_time
    ffengc_log::logSink::ptr stdout_lsp = ffengc_log::sinkFactory::create<ffengc_log::stdoutSink>();
    ffengc_log::logSink::ptr file_lsp = ffengc_log::sinkFactory::create<ffengc_log::fileSink>("./logfile/test.log");
    ffengc_log::logSink::ptr roll_lsp = ffengc_log::sinkFactory::create<ffengc_log::rollSink>("./logfile/roll-", 1024 * 1024);
#endif
    ffengc_log::logSink::ptr roll_time_lsp = ffengc_log::sinkFactory::create<extension::rollSinkByTime>("./logfile/roll-time-", extension::timeGap::GAP_SECOND);
#if !__test_extension_roll_sink_by_time
    stdout_lsp->log(str.c_str(), str.size());
    file_lsp->log(str.c_str(), str.size());
    size_t cur_size = 0;
    size_t cnt = 0;
    while (cur_size < 1024 * 1024 * 10) {
        std::string tmp = str + std::to_string(cnt++);
        roll_lsp->log(tmp.c_str(), tmp.size());
        cur_size += tmp.size();
    }
#endif
    time_t old = ffengc_log::util::Date::now();
    while (ffengc_log::util::Date::now() < old + 5) {
        roll_time_lsp->log(str.c_str(), str.size());
    }
}
TEST(all_test, sync_logger_test) {
    std::string logger_name = "sync_logger";
    ffengc_log::logLevel::value limit = ffengc_log::logLevel::value::WARNING;
    ffengc_log::formatter::ptr fmt(new ffengc_log::formatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n"));
    // 3个日志器
    std::vector<ffengc_log::logSink::ptr> sinks = {
        ffengc_log::sinkFactory::create<ffengc_log::stdoutSink>(),
        ffengc_log::sinkFactory::create<ffengc_log::fileSink>("./logfile/test.log"),
        ffengc_log::sinkFactory::create<ffengc_log::rollSink>("./logfile/roll-", 1024 * 1024)
    };
    ffengc_log::logger::ptr logger(new ffengc_log::syncLogger(logger_name, limit, fmt, sinks));
    size_t cur_size = 0, count = 0;
    std::string str = "log test from sync_logger_test";
    logger->debug(__FILE__, __LINE__, "%s", str.c_str()); // 应该是输出不了的
    logger->info(__FILE__, __LINE__, "%s", str.c_str());
    logger->warning(__FILE__, __LINE__, "%s", str.c_str());
    logger->error(__FILE__, __LINE__, "%s", str.c_str());
    logger->fatal(__FILE__, __LINE__, "%s", str.c_str());

    // while (cur_size < 1024 * 1024 * 10) {
    //     std::string tmp = "[" + std::to_string(count++) + "]" + str;
    //     logger->fatal(__FILE__, __LINE__, "%s", tmp.c_str());
    //     cur_size += tmp.size();
    // }
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new all_test);
    testing::GTEST_FLAG(filter) = "all_test.sync_logger_test";
    return RUN_ALL_TESTS();
}