# Multi-Pattern-Logging-System

- [简体中文](./README-cn.md)
- [English](./README.md)

C++ version of synchronous &amp; asynchronous logging system based on multiple design patterns

> [!TIP]
> If you have any questions about the operation and implementation of this project, or if you have better optimization suggestions for this project, you can contact me directly or leave an issue in the repository.

## Basic information of the project

This project is mainly to implement a log system, which mainly supports the following functions:

- Support multi-level log messages
- Support synchronous logs and asynchronous logs
- Support reliable writing of logs to the console, files and rolling files
- Support multi-threaded programs to write logs concurrently
- Support the expansion of different log landing destinations

Core technology stack: class hierarchy design (inheritance and polymorphism), C++11 (multi-threading, right value references, etc.), double buffer, producer consumer model, multi-threading, design pattern (singleton, factory proxy, builder, etc.), GTest

> [!WARNING]
> **This is a component-based project, not a business-based project, and it will eventually be used by others.**


## How to use

- [Documentation of the Log System](./docs/documentation-en.md)

## Performance Testing

**Test environment:**
- Virtual Machine: Parallels Desktop, Ubuntu22.04 VirtualHost Memory 2G CPU Dual Core
- Host: Macbook Air M2 Memory 16G

For how to use the `bench` code, see `bench/bench.cc`.

**Test method:** Each configuration was tested 10 times and the average result was taken. The results are shown below.

| MODE         | Thead Number | Message Size (KB) | Message per second (M/s) | Output Size per second (MB/s) |
| ------------ | ------------ | ----------------- | ------------------------ | ----------------------------- |
| LOGGER_SYNC  | 1            | 976562(100 items) | 1.60668                  | 153.224                       |
| LOGGER_SYNC  | 10           | 976562            | 1.80325                  | 171.971                       |
| LOGGER_ASYNC | 1            | 976562            | 1.30354                  | 124.314                       |
| LOGGER_ASYNC | 10           | 976562            | 3.15184                  | 300.582                       |

> [!TIP]
> The test results also show that in multi-threaded mode, the asynchronous working mode greatly improves performance.


## Technical supplement

To complete this project, I used a document to supplement the detailed explanation of indeterminate parameter functions and six design patterns as a prerequisite technical supplement for this project.

- [Prerequisite knowledge document for the project](./docs/supplement-en.md)

## Specific implementation of the project

The specific structure of the project is shown below.

![](./assets/1.png)

For detailed implementation, see the following documents:

- [word.md](./docs/work.md) (Chinese Only)

## Some ideas

- Encapsulate into a library
- Understand how to encapsulate into interfaces of other languages, such as Python and C#, and try it. See if you can call the library in Python/C# to use this log system.