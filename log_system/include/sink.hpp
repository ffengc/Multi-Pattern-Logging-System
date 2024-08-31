/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_SINK__
#define __YUFC_SINK__

#include "util.hpp"
#include <assert.h>
#include <fstream>
#include <memory>
#include <sstream>

namespace ffengc_log {
class logSink {
public:
    using ptr = std::shared_ptr<logSink>;
    logSink() = default;
    virtual ~logSink() { }
    virtual void log(const char* data, size_t len) = 0;
};
// 标准输出
class stdoutSink : public logSink {
public:
    void log(const char* data, size_t len) { std::cout.write(data, len); }
};
// 指定文件
class fileSink : public logSink {
private:
    std::ofstream __ofs;
    std::string __file_name; //
public:
    fileSink(const std::string& file_name)
        : __file_name(file_name) {
        // 1. 创建日志文件所在的目录
        util::File::createDirectory(util::File::path(file_name));
        // 2. 创建并打开日志文件
        __ofs.open(__file_name, std::ios::binary | std::ios::app);
        assert(__ofs.is_open());
    }
    void log(const char* data, size_t len) {
        __ofs.write(data, len);
        assert(__ofs.good());
    }
};
// 滚动文件（以大小进行滚动）
class rollSink : public logSink {
private:
    std::string __base_name; // {./log/base-}xxx.log
    std::ofstream __ofs;
    size_t __max_size; // 滚动的大小阈值
    size_t __cur_fsize; // 记录当前大小，避免重复查询文件状态（效率很低）
    size_t __name_cnt; // 文件名计数器
public:
    rollSink(const std::string& base_name, size_t max_size)
        : __base_name(base_name)
        , __max_size(max_size)
        , __name_cnt(0)
        , __cur_fsize(0) {
        std::string path_name = createNewFile();
        util::File::createDirectory(util::File::path(path_name));
        __ofs.open(path_name, std::ios::binary | std::ios::app);
        assert(__ofs.is_open());
    }
    void log(const char* data, size_t len) {
        if (__cur_fsize >= __max_size) {
            __ofs.close();
            std::string path_name = createNewFile();
            __ofs.open(path_name, std::ios::binary | std::ios::app);
            assert(__ofs.is_open());
            __cur_fsize = 0;
        }
        __ofs.write(data, len);
        assert(__ofs.good());
        __cur_fsize += len;
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
        file_name << "-";
        file_name << __name_cnt++;
        file_name << ".log";
        return file_name.str();
    }
};
// easy factory mode
class sinkFactory {
public:
    template <typename SinkType, typename... Args>
    static logSink::ptr create(Args&&... args) {
        return std::make_shared<SinkType>(std::forward<Args>(args)...);
    }
};
} // namespace ffengc_log

#endif