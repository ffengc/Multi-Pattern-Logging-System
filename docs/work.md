# 项目详细实现

- [项目详细实现](#项目详细实现)
  - [框架设计](#框架设计)
  - [工具类编写](#工具类编写)
  - [日志等级模块](#日志等级模块)
  - [日志消息类](#日志消息类)
  - [消息格式化模块](#消息格式化模块)

## 框架设计

- 日志登记模块：枚举出日志分为多少个等级，对不同的日志需要有不同的等级标记
- 日志消息模块：封装一条日志所需的各种要素（时间，线程ID，文件名，行号，日志等级，消息主题...）
- 消息格式化模块：按照指定的格式，对日志消息关键要素进行组织，最终得到一个指定格式的字符串
- 日志落地模块：对上面的这几个模块进行整合
  - 同时还需要增加，同步日志器模块和异步日志器模块
- 异步线程模块：负责异步日志的实际落地输出功能
- 单例的日志器管理模块：对日志进行全局的管理，以便能够在项目的任何位置获取指定的日志器进行日志输出

## 工具类编写

**具体需要以下功能：**
- 获取时间
- 判断文件（目录）是否存在
- 获取文件的父级目录
- 创建指定路径的文件夹

代码如下所示：
```cpp
namespace ffengc_log {
namespace util {
    class Date {
    public:
        static size_t getTime() { return (size_t)time(nullptr); }
    };
    class File {
    public:
        static bool exists(const std::string& path_name) {
            struct stat st;
            if (stat(path_name.c_str(), &st) < 0)
                return false;
            return true;
        }
        static std::string path(const std::string& path_name) {
            // ./abc/a.txt
            auto pos = path_name.find_last_of("/\\");
            if (pos == std::string::npos)
                return ".";
            return path_name.substr(0, pos + 1);
        }
        static void createDirectory(const std::string& path_name) {
            // ./abc/bcd/a.txt
            size_t pos = 0, idx = 0;
            while (idx < path_name.size()) {
                pos = path_name.find_first_of("/\\", idx);
                if (pos == std::string::npos)
                    mkdir(path_name.c_str(), 0777);
                std::string parent_dir = path_name.substr(0, pos - idx + 1);
                // if (parent_dir == "." || parent_dir == "..") {
                //     idx = pos + 1;
                //     continue;
                // }
                if (exists(parent_dir)) {
                    idx = pos + 1;
                    continue;
                }
                mkdir(parent_dir.c_str(), 0777);
                idx = pos + 1;
            }
        }
    };
} // namespace util
} // namespace ffengc_log
```

然后我会使用 `gtest` 框架来对 `util` 进行单元测试。

## 日志等级模块

具体实现和等级设置如下所示。

```cpp
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
    const char* toString(logLevel::value level) {
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
```

## 日志消息类

**需要输出如下要素：**
- 日志的输出时间：用于过滤日志输出时间
- 日志等级：用于进行日志过滤分析
- 源文件名称
- 源文件行号：用于定位出现错误的代码位置
- 线程ID：用于过滤出错的线程
- 日志主体消息
- 日志器名称：当前支持多个日志器同时使用

代码如下所示:

```cpp
namespace ffengc_log {
struct logMessage {
    size_t __ctime; // 日志产生的时间戳
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
```

## 消息格式化模块

现在需要把 `logMessage` 里面的信息，组织成一个字符串。

这里要明确，为了用户可以自己设置到底需要输出多少信息，所以我们要定义指定的输出格式，如下所示。


| 符号 | 描述       |
| ---- | ---------- |
| `%d` | 日期       |
| `%T` | 缩进       |
| `%t` | 线程id     |
| `%p` | 日志级别   |
| `%c` | 日志器名称 |
| `%f` | 文件名     |
| `%l` | 行号       |
| `%m` | 日志消息   |
| `%n` | 换行       |

这样每个 `%...` 可以对应一个信息，这样才能让用户去指定，到底需要输出多少信息，而不是全部统一输出固定的信息。

需要的成员：
- 格式化字符串
- 格式化子项数组：不同的格式化子项，会从日志消息中取出指定的元素，转换为字符串

格式化子项数组 `std::vector<FormatItem::ptr> items` 成员：用于按序保存格式化字符串对应的子格式化对象。`FormatItem` 类主要负责日志消息子项的获取及格式化。


| 类名               | 描述                                 |
| ------------------ | ------------------------------------ |
| `MsgFormatItem`    | 表示要从`LogMsg`中取出有效日志数据   |
| `LevelFormatItem`  | 表示要从`LogMsg`中取出日志等级       |
| `NameFormatItem`   | 表示要从`LogMsg`中取出日志器名称     |
| `ThreadFormatItem` | 表示要从`LogMsg`中取出线程ID         |
| `TimeFormatItem`   | 表示要从`LogMsg`中取出时间戳         |
| `CFileFormatItem`  | 表示要从`LogMsg`中取出源码所在文件名 |
| `CLineFormatItem`  | 表示要从`LogMsg`中取出源码所在行号   |
| `TabFormatItem`    | 表示一个制表符缩进                   |
| `NLineFormatItem`  | 表示一个换行                         |
| `OtherFormatItem`  | 表示非格式化的原始字符串             |

具体实现见代码所示。

