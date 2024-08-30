/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#include "format.hpp"
#include "level.hpp"
#include "message.hpp"
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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new all_test);
    testing::GTEST_FLAG(filter) = "all_test.format_test";
    return RUN_ALL_TESTS();
}