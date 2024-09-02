/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#include "internal/buffer.hpp"
#include "internal/format.hpp"
#include "internal/level.hpp"
#include "internal/logger.hpp"
#include "internal/message.hpp"
#include "internal/sink.hpp"
#include "internal/util.hpp"
#include <gtest/gtest.h>

#define sink_extension false

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
#if sink_extension
    ffengc_log::logSink::ptr roll_time_lsp = ffengc_log::sinkFactory::create<extension::rollSinkByTime>("./logfile/roll-time-", extension::timeGap::GAP_SECOND);
#endif
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
#if sink_extension
    time_t old = ffengc_log::util::Date::now();
    while (ffengc_log::util::Date::now() < old + 5) {
        roll_time_lsp->log(str.c_str(), str.size());
    }
#endif
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

TEST(all_test, sync_logger_builder_test) {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::localLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("sync_logger");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_SYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::fileSink>("./logfile/test.log");
    auto logger = builder->build();
    std::string str = "log test from sync_logger_builder_test";
    logger->debug(__FILE__, __LINE__, "%s", str.c_str()); // 应该是输出不了的
    logger->info(__FILE__, __LINE__, "%s", str.c_str());
    logger->warning(__FILE__, __LINE__, "%s", str.c_str());
    logger->error(__FILE__, __LINE__, "%s", str.c_str());
    logger->fatal(__FILE__, __LINE__, "%s", str.c_str());
}
TEST(all_test, single_buffer_test) {
    // 读取文件数据，一点一点写入缓冲区，最终将缓冲区数据写入文件，判断生成的新文件与源文件是否一致
    std::ifstream ifs("./logs/test.log", std::ios::binary);
    if (ifs.is_open() == false) {
        std::cerr << "open file failed" << std::endl;
        abort();
    }
    ifs.seekg(0, std::ios::end); // 跳转到文件末尾
    size_t fsize = ifs.tellg(); // 获取当前读写位置相对于起始位置的偏移量
    ifs.seekg(0, std::ios::beg); // 重新跳转到起始位置
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);
    if (ifs.good() == false) {
        std::cerr << "read error" << std::endl;
        abort();
    }
    ifs.close();

    std::cout << fsize << std::endl;
    ffengc_log::buffer bff;
    for (int i = 0; i < body.size(); ++i) {
        // std::cout << bff.__read_idx << ":" << bff.__write_idx << std::endl;
        bff.push(&body[i], 1);
    }
    std::cout << bff.readableSize() << std::endl;
    std::ofstream ofs("./logs/temp.log", std::ios::binary);
    // ofs.write(bff.begin(), bff.readableSize());
    size_t write_size = bff.readableSize();
    for (int i = 0; i < write_size; ++i) {
        ofs.write(bff.begin(), 1);
        if (!ofs.good()) {
            std::cerr << "ofs error" << std::endl;
            abort();
        }
        bff.moveReader(1);
    }
    ofs.close();
}

TEST(all_test, async_log_test) {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::localLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("async_logger");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/async_test.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger = builder->build();
    std::string str = "log test from async_log_test";
    logger->debug(__FILE__, __LINE__, "%s", str.c_str()); // 应该是输出不了的
    logger->info(__FILE__, __LINE__, "%s", str.c_str());
    logger->warning(__FILE__, __LINE__, "%s", str.c_str());
    logger->error(__FILE__, __LINE__, "%s", str.c_str());
    logger->fatal(__FILE__, __LINE__, "%s", str.c_str());
    size_t count = 0;
    while (count < 500000) {
        std::string tmp = "[" + std::to_string(count++) + "]" + str;
        logger->fatal(__FILE__, __LINE__, "%s", tmp.c_str());
    }
}

void test_log() {
    ffengc_log::logger::ptr logger = ffengc_log::loggerManager::getInstance().get("globalLoggerBuilder");
    std::string str = "log test from logManager_test";
    logger->debug(__FILE__, __LINE__, "%s", str.c_str()); // 应该是输出不了的
    logger->info(__FILE__, __LINE__, "%s", str.c_str());
    logger->warning(__FILE__, __LINE__, "%s", str.c_str());
    logger->error(__FILE__, __LINE__, "%s", str.c_str());
    logger->fatal(__FILE__, __LINE__, "%s", str.c_str());
}
TEST(all_test, globalLoggerBuilder) {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("globalLoggerBuilder");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger = builder->build();
    test_log();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new all_test);
    testing::GTEST_FLAG(filter) = "all_test.globalLoggerBuilder";
    return RUN_ALL_TESTS();
}