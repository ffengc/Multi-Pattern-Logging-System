# 使用文档

- [使用文档](#使用文档)
  - [C/C++](#cc)
    - [使用默认日志器](#使用默认日志器)
    - [自定义日志器](#自定义日志器)

## C/C++

克隆仓库后，将 `base/internal` 目录和 `base/log.h` 目录拷贝到合适的位置下。

编译时带上 `log.h` 的路径并直接包含该头文件。

例如 `example/makefile`

```makefile
CFLAG= -I../base/
LFLAG= -lpthread
example.out: example.cc
	g++ -g -std=c++11 $(CFLAG) $^ -o $@  $(LFLAG)
.PHONY:clean
clean:
	rm -rf example.out logfile
```

> [!IMPORTANT]
> 编译时需要链接线程库

### 使用默认日志器

默认日志器将在 `stdout` 中直接打印日志信息，并且所有等级的日志都会进行输出。

宏接口有:
```cpp
#define LOG_DEBUG(name, fmt, ...) ffengc_log::getLogger(name)->debug(fmt, ##__VA_ARGS__);
#define LOG_INFO(name, fmt, ...) ffengc_log::getLogger(name)->info(fmt, ##__VA_ARGS__);
#define LOG_WARNING(name, fmt, ...) ffengc_log::getLogger(name)->warning(fmt, ##__VA_ARGS__);
#define LOG_ERROR(name, fmt, ...) ffengc_log::getLogger(name)->error(fmt, ##__VA_ARGS__);
#define LOG_FATAL(name, fmt, ...) ffengc_log::getLogger(name)->fatal(fmt, ##__VA_ARGS__);
```

例子(`example/example.cc:use_default_logger()`):
```cpp
void use_default_logger() {
    DLOG_DEBUG("hello world, %d", 3);
    DLOG_INFO("hello world, %s", "hello linux");
    DLOG_WARNING("hello world, %p", (char*)malloc(10));
    DLOG_ERROR("hello world, %.2f", 0.313);
    DLOG_FATAL("hello world");
}
```
输出:
```sh
[14:46:20][281473887660576][root][example.cc:30][DEBUG] hello world, 3
[14:46:20][281473887660576][root][example.cc:31][INFO] hello world, hello linux
[14:46:20][281473887660576][root][example.cc:32][WARNING] hello world, 0xaaab062f1c40
[14:46:20][281473887660576][root][example.cc:33][ERROR] hello world, 0.31
[14:46:20][281473887660576][root][example.cc:34][FATAL] hello world
```

> [!TIP]
> **默认日志器输出格式如下所示:**
> `[%d{%H:%M:%S}][%t][%c][%f:%l][%p] %m%n`
>  * %d 表示日期，包含子格式 `{%H:%M:%S}`
>  * %t 表示线程ID
>  * %c 表示日志器名称
>  * %f 表示源码文件名
>  * %l 表示源码行号
>  * %p 表示日志级别
>  * %T 表示制表符缩进
>  * %m 表示主体消息
>  * %n 标识换行


### 自定义日志器

自定义日志器需要编写代码进行日志器的配置。

自定义日志器代码如下所示。

```cpp
void init_logger() {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("this project");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger_obj = builder->build();
}
```

首先需要定义一个日志器的`builder`对象，例子上我使用了智能指针进行管理

```cpp
std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
```

**1. 构建`builer`对象**

`loggerBuilder` 有两种，一种是 `localLoggerBuilder`, 另一种是 `globalLoggerBuilder`

- `localLoggerBuilder` 创建的日志器只能在当前作用域使用
- `globalLoggerBuilder` 创建的日志器可以在全局使用（为单例）

**2. 指定日志器名称**

```cpp
builder->buildLoggerName("this project");
```

**3. 指定日志器输出等级限制**

```cpp
builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
```
表示仅有 `WARNING` 及其以上的日志等级会被输出。

有这些等级: `DEBUG, INFO, WARNING, ERROR, FATAL`

**4. 指定日志器模式（同步/异步）**

```cpp
builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
// ffengc_log::loggerType::LOGGER_ASYNC and ffengc_log::loggerType::LOGGER_SYNC
```

> [!TIP]
> **异步日志的输出阻塞不会阻塞主线程的运行**

**5. 指定日志输出格式**

```cpp
builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
```

**`buildFormatter` 如果不传参则表示使用默认输出格式。**

> [!TIP]
> **默认输出格式如下所示:**
> `[%d{%H:%M:%S}][%t][%c][%f:%l][%p] %m%n`
>  * `%d` 表示日期，包含子格式 `{%H:%M:%S}`
>  * `%t` 表示线程ID
>  * `%c` 表示日志器名称
>  * `%f` 表示源码文件名
>  * `%l` 表示源码行号
>  * `%p` 表示日志级别
>  * `%T` 表示制表符缩进
>  * `%m` 表示主体消息
>  * `%n` 标识换行


**6. 指定日志器输出方向**

有三种输出方式:

- `stdout`输出
- 文件输出：该日志器所有输出都输出至单个文件中
- 滚动文件输出：该日志器的所有输出会输出到一系列文件当中，当前设置为，若日志文件达到一定大小，则切换到新文件中。

```cpp
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
```

```cpp
builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
```
这一句代码中，第一个参数表示文件名的前缀路径，完整路径会被扩展加上时间信息，为 `./logfile/async_test_roll-[timestamp].log`，第二个参数为单个文件大小。

当然可以扩展日志器的输出方向，具体见 `example/extension_rollSinkbyTime.hpp`，为根据时间进行文件滚动的扩展代码。

单个日志器可以指定多个Sink方向。

**7. 构建日志器**

```cpp
auto logger = builder->build();
```

**8. 全局日志器的使用**

```cpp
void init_logger() {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("this project");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger = builder->build();
}
void use_config_logger() {
    LOG_FATAL("this project", "hello world");
}
```
输出如下所示。

```sh
[15:11:33][this project][example.cc:39][FATAL]  hello world
```

**9. 局部日志器的使用**

```cpp
void use_local_config_logger() {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::localLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("local logger");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
    auto logger = builder->build();
    logger->fatal("local logger", "hello world");
}
```

> [!CAUTIONS]
> 局部日志器不能调用宏！只能通过局部的 `logger` 对象来显示调用对应的日志函数，如代码所示。