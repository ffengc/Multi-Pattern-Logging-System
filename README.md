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

Core technology stack: class hierarchy design (inheritance and polymorphism), C++11 (multi-threading, right value references, etc.), double buffer, producer consumer model, multi-threading, design pattern (singleton, factory proxy, builder, etc.)

> [!WARNING]
> **This is a component-based project, not a business-based project, and it will eventually be used by others.**

## Technical supplement

To complete this project, I used a document to supplement the detailed explanation of indeterminate parameter functions and six design patterns as a prerequisite technical supplement for this project.

- [Prerequisite knowledge document for the project](./docs/supplement.md)

## Some ideas

- Encapsulate into a library
- Understand how to encapsulate into interfaces of other languages, such as Python and C#, and try it. See if you can call the library in Python/C# to use this log system