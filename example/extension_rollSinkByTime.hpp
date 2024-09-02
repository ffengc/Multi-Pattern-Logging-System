/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_EXTENSION_ROLLSINK_BY_TIME__
#define __YUFC_EXTENSION_ROLLSINK_BY_TIME__

#include "log.h"
// 用户扩展落地方式
// 以时间段进行滚动

namespace extension {

using namespace ffengc_log;

enum class timeGap {
    GAP_SECOND,
    GAP_MINUTE,
    GAP_HOUR,
    GAP_DAY,
};
class rollSinkByTime : public logSink {
private:
    std::string __base_name; // {./log/base-}xxx.log
    std::ofstream __ofs;
    size_t __cur_gap; // 当前是第几个时间段
    size_t __gap_size; // 时间段的大小
public:
    rollSinkByTime(const std::string& base_name, timeGap gap_type)
        : __base_name(base_name) {
        switch (gap_type) {
        case timeGap::GAP_SECOND:
            __gap_size = 1;
            break;
        case timeGap::GAP_MINUTE:
            __gap_size = 60;
            break;
        case timeGap::GAP_HOUR:
            __gap_size = 3600;
            break;
        case timeGap::GAP_DAY:
            __gap_size = 3600 * 24;
            break;
        default:
            assert(false);
        }
        std::string file_name = createNewFile();
        util::File::createDirectory(util::File::path(file_name));
        __ofs.open(file_name, std::ios::binary | std::ios::app);
        assert(__ofs.is_open());
        __cur_gap = __gap_size == 1 ? util::Date::now() : util::Date::now() % __gap_size;
    }
    void log(const char* data, size_t len) {
        time_t cur = util::Date::now();
        if ((__gap_size == 1 ? (cur != __cur_gap) : (cur % __gap_size != __cur_gap))) {
            // 这里的特殊处理要注意 
            __ofs.close();
            std::string file_name = createNewFile();
            __ofs.open(file_name, std::ios::binary | std::ios::app);
            assert(__ofs.is_open());
            __cur_gap = __gap_size == 1 ? util::Date::now() : util::Date::now() % __gap_size;
        }
        __ofs.write(data, len);
        assert(__ofs.good());
    } //
private:
    std::string createNewFile() {
        // 获取系统时间，以时间来构造文件名扩展名
        time_t t = util::Date::now();
        struct tm lt;
        localtime_r(&t, &lt);
        std::stringstream file_name;
        file_name << __base_name;
        file_name << lt.tm_year + 1900;
        file_name << lt.tm_mon + 1;
        file_name << lt.tm_mday;
        file_name << lt.tm_hour;
        file_name << lt.tm_min;
        file_name << lt.tm_sec;
        file_name << ".log";
        return file_name.str();
    }
};
}
#endif