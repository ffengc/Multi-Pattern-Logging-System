# Documentation

- [简体中文](./documentation.md)
- [English](./documentation-en.md)

***
- [Documentation](#documentation)
  - [C/C++](#cc)
    - [Use the default logger](#use-the-default-logger)
    - [Custom logger](#custom-logger)
  - [Python](#python)
  - [C#](#c)

## C/C++

After cloning the repository, copy the `base/internal` directory and the `base/log.h` directory to the appropriate location.

When compiling, bring the path to `log.h` and include the header file directly.

For example, `example/makefile`

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
> Need to link thread library when compiling

### Use the default logger

The default logger will print log information directly to `stdout`, and all log levels will be output.

The macro interfaces are:
```cpp
#define LOG_DEBUG(name, fmt, ...) ffengc_log::getLogger(name)->debug(fmt, ##__VA_ARGS__);
#define LOG_INFO(name, fmt, ...) ffengc_log::getLogger(name)->info(fmt, ##__VA_ARGS__);
#define LOG_WARNING(name, fmt, ...) ffengc_log::getLogger(name)->warning(fmt, ##__VA_ARGS__);
#define LOG_ERROR(name, fmt, ...) ffengc_log::getLogger(name)->error(fmt, ##__VA_ARGS__);
#define LOG_FATAL(name, fmt, ...) ffengc_log::getLogger(name)->fatal(fmt, ##__VA_ARGS__);
```

example(`example/example.cc:use_default_logger()`):
```cpp
void use_default_logger() {
    DLOG_DEBUG("hello world, %d", 3);
    DLOG_INFO("hello world, %s", "hello linux");
    DLOG_WARNING("hello world, %p", (char*)malloc(10));
    DLOG_ERROR("hello world, %.2f", 0.313);
    DLOG_FATAL("hello world");
}
```
output:
```sh
[14:46:20][281473887660576][root][example.cc:30][DEBUG] hello world, 3
[14:46:20][281473887660576][root][example.cc:31][INFO] hello world, hello linux
[14:46:20][281473887660576][root][example.cc:32][WARNING] hello world, 0xaaab062f1c40
[14:46:20][281473887660576][root][example.cc:33][ERROR] hello world, 0.31
[14:46:20][281473887660576][root][example.cc:34][FATAL] hello world
```

> [!TIP]
> **The default logger output format is as follows:**
> `[%d{%H:%M:%S}][%t][%c][%f:%l][%p] %m%n`
> * %d indicates the date, including the subformat `{%H:%M:%S}`
> * %t indicates the thread ID
> * %c indicates the logger name
> * %f indicates the source code file name
> * %l indicates the source code line number
> * %p indicates the log level
> * %T indicates the tab indentation
> * %m indicates the main message
> * %n indicates the line break


### Custom logger

Custom logger requires writing code to configure the logger.

The custom logger code is as follows.

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

First, you need to define a logger's `builder` object. In the example, I use smart pointers to manage it.

```cpp
std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
```

**1. Build `builer` object**

There are two types of `loggerBuilder`, one is `localLoggerBuilder`, the other is `globalLoggerBuilder`

- The logger created by `localLoggerBuilder` can only be used in the current scope
- The logger created by `globalLoggerBuilder` can be used globally (singleton)

**2. Specify the logger name**

```cpp
builder->buildLoggerName("this project");
```

**3. Specify the logger output level limit**

```cpp
builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
```
Indicates that only log levels of `WARNING` and above will be output.

These levels are: `DEBUG, INFO, WARNING, ERROR, FATAL`

**4. Specify the logger mode (synchronous/asynchronous)**

```cpp
builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
// ffengc_log::loggerType::LOGGER_ASYNC and ffengc_log::loggerType::LOGGER_SYNC
```

> [!TIP]
> **Blocking the output of asynchronous logs will not block the operation of the main thread**

**5. Specify the log output format**

```cpp
builder->buildFormatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n");
```

**`buildFormatter` 如果不传参则表示使用默认输出格式。**

> [!TIP]
> **The default output format is as follows:**
> `[%d{%H:%M:%S}][%t][%c][%f:%l][%p] %m%n`
> * `%d` indicates the date, including the subformat `{%H:%M:%S}`
> * `%t` indicates the thread ID
> * `%c` indicates the logger name
> * `%f` indicates the source code file name
> * `%l` indicates the source code line number
> * `%p` indicates the log level
> * `%T` indicates the tab indentation
> * `%m` indicates the main message
> * `%n` indicates the line break


**6. Specify the logger output direction**

There are three output modes:

- `stdout` output
- File output: All output of this logger is output to a single file
- Rolling file output: All output of this logger is output to a series of files. The current setting is that if the log file reaches a certain size, it will switch to a new file.

```cpp
    builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
    builder->buildSink<ffengc_log::fileSink>("./logfile/globalLoggerBuilder.log");
    builder->buildSink<ffengc_log::stdoutSink>();
```

```cpp
builder->buildSink<ffengc_log::rollSink>("./logfile/async_test_roll-", 1024 * 1024);
```
In this code, the first parameter indicates the prefix path of the file name. The full path will be extended with time information, which is `./logfile/async_test_roll-[timestamp].log`. The second parameter is the size of a single file.

Of course, the output direction of the logger can be extended. For details, see `example/extension_rollSinkbyTime.hpp`, which is the extension code for file rolling based on time.

A single logger can specify multiple Sink directions.

**7. Build a logger**

```cpp
auto logger = builder->build();
```

**8. Use of the global logger**

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

The output will be as shown below.

```sh
[15:11:33][this project][example.cc:39][FATAL]  hello world
```

**9. Use of local logger**

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
> Local loggers cannot call macros! You can only explicitly call the corresponding log function through the local `logger` object, as shown in the code.

## Python

To be expanded.

## C#

To be expanded.