/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_FORMAT__
#define __YUFC_FORMAT__

#include "level.hpp"
#include "message.hpp"
#include <assert.h>
#include <memory>
#include <sstream>
#include <time.h>
#include <unordered_map>
#include <vector>

namespace ffengc_log {
// 抽象格式化子项类
class formatItem {
public:
    using ptr = std::shared_ptr<formatItem>;
    virtual void format(std::ostream& out, const logMessage& msg) = 0;
};
// 派生格式化子项子类 -- 消息、等级、时间、文件名、行号、线程id、日志器名称、制表符、换行、其他
class messageFormatItem : public formatItem {
public:
    messageFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) { out << msg.__payload; }
};
class levelFormatItem : public formatItem {
public:
    levelFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << logLevel::toString(msg.__level); }
};
class timeFormatItem : public formatItem {
public:
    timeFormatItem(const std::string& fmt = "%H:%M:%S")
        : __time_fmt(fmt) {
        if (fmt.empty())
            __time_fmt = "%H:%M:%S";
    }
    void format(std::ostream& out, const logMessage& msg) override {
        struct tm t;
        localtime_r(&msg.__ctime, &t);
        char tmp[128] = { 0 };
        strftime(tmp, 31, __time_fmt.c_str(), &t);
        out << tmp;
    } //
private:
    std::string __time_fmt; // %H:%M:%S
};
class fileFormatItem : public formatItem {
public:
    fileFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << msg.__file; }
};
class lineFormatItem : public formatItem {
public:
    lineFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << msg.__line; }
};
class threadIdFormatItem : public formatItem {
public:
    threadIdFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << msg.__tid; }
};
class loggerFormatItem : public formatItem {
public:
    loggerFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << msg.__logger; }
};
class tabFormatItem : public formatItem {
public:
    tabFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const logMessage& msg) override { out << "\t"; }
};
class newLineFormatItem : public formatItem {
public:
    newLineFormatItem(const std::string& str = "") { }
    void format(std::ostream& out, const  logMessage& msg) override { out << "\n"; }
};
class otherFormatItem : public formatItem {
public:
    otherFormatItem(const std::string& str)
        : __str(str) { }
    void format(std::ostream& out, const logMessage& msg) override { out << __str; } //
private:
    std::string __str;
};
// 格式化器
/**
 * %d 表示日期，包含子格式 {%H:%M:%S}
 * %t 表示线程ID
 * %c 表示日志器名称
 * %f 表示源码文件名
 * %l 表示源码行号
 * %p 表示日志级别
 * %T 表示制表符缩进
 * %m 表示主体消息
 * %n 标识换行
 */
class formatter {
private:
    std::string __pattern; // 格式化规则字符串
    std::vector<formatItem::ptr> __items; //
public:
    using ptr = std::shared_ptr<formatter>;
    formatter(const std::string& pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
        : __pattern(pattern) {
        assert(parsePattern());
    }
    // 对msg进行格式化
    void format(std::ostream& out, const logMessage& msg) {
        for (const auto& item : __items)
            item->format(out, msg);
    }
    std::string format(const logMessage& msg) {
        std::stringstream ss;
        format(ss, msg);
        return ss.str();
    } //
private:
    // 对格式化规则字符串进行解析
    bool parsePattern() {
        // 1. 对格式化规则字符串进行解析
        // 没有以%起始的字符串都是原始字符串
        // 处理思想：不是%，就一直走，直到遇到%，则是原始字符串的结束
        // 如果遇到%%，就代表是一个原始字符%，否则%...表示格式化字符
        // 如果格式化字符串%x后面跟着的是{，则表示{}跟着的是子格式
        std::vector<std::pair<std::string, std::string>> order;
        std::string key, val;
        size_t pos = 0;
        while (pos < __pattern.size()) {
            if (__pattern[pos] != '%') {
                val.push_back(__pattern[pos++]);
                continue;
            }
            // pos 位置是 %
            if (pos + 1 < __pattern.size() && __pattern[pos + 1] == '%') {
                // %% 的情况
                val.push_back('%');
                pos += 2;
                continue;
            }
            // 这里代表原始字符串处理完毕
            if (val.empty() == false)
                order.push_back({ "", val });
            val.clear();

            // 走到这里代表是一个格式化字符
            pos += 1;
            if (pos == __pattern.size()) {
                std::cerr << "there is no fmt str after the %, error" << std::endl;
                return false;
            }
            key = __pattern[pos];
            // 这个时候pos指向格式化字符后的位置
            pos += 1;
            if (pos < __pattern.size() && __pattern[pos] == '{') {
                pos += 1; // pos指向子规则的起始位置
                while (pos < __pattern.size() && __pattern[pos] != '}')
                    val.push_back(__pattern[pos++]);
                if (pos == __pattern.size()) {
                    return false; // 走到末尾跳出循环，则代表没有遇到}，表示格式错误
                    std::cerr << "substr '{}', fmt error" << std::endl; // for debug
                }
                pos += 1;
            }
            order.push_back({ key, val });
            key.clear();
            val.clear();
        }
        // 2. 根据解析得到的数据初始化格式化子项数组成员
        for (const auto& it : order)
            __items.push_back(createItem(it.first, it.second));
        return true;
    } //
    // 根据不同的格式化字符创建不同的格式化子项对象
    formatItem::ptr createItem(const std::string& key, const std::string& val) {
        if (key == "d")
            return formatItem::ptr(new timeFormatItem(val));
        if (key == "t")
            return formatItem::ptr(new threadIdFormatItem(val));
        if (key == "c")
            return formatItem::ptr(new loggerFormatItem(val));
        if (key == "f")
            return formatItem::ptr(new fileFormatItem(val));
        if (key == "l")
            return formatItem::ptr(new lineFormatItem(val));
        if (key == "p")
            return formatItem::ptr(new levelFormatItem(val));
        if (key == "T")
            return formatItem::ptr(new tabFormatItem(val));
        if (key == "m")
            return formatItem::ptr(new messageFormatItem(val));
        if (key == "n")
            return formatItem::ptr(new newLineFormatItem(val));
        if (key == "")
            return formatItem::ptr(new otherFormatItem(val));
        std::cerr << "this is not a valid fmt char: %" << key << std::endl;
        abort();
    }
};

} // namespace ffengc_log

#endif