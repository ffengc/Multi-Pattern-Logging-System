/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#include "log.h"
#include <algorithm>
#include <chrono>
#include <numeric>

std::pair<double, double> bench(const std::string& logger_name, size_t thr_count, size_t msg_count, size_t msg_len) {
    // 1. 获取日志器
    ffengc_log::logger::ptr obj = ffengc_log::getLogger(logger_name);
    if (obj == nullptr)
        return {};
    std::cout << "test, msg_count: " << msg_count << " total_size: " << (msg_count * msg_len) / 1024 << "kb" << std::endl;
    // 2. 组织制定长度的日志消息
    std::string msg(msg_len - 1, 'A');
    // 3. 创建指定数量的线程
    std::vector<std::thread> threads;
    std::vector<double> cost_array(thr_count);
    size_t msg_per_thread = msg_count / thr_count; // 每个线程要输出日志的数量
    for (int i = 0; i < thr_count; ++i) {
        threads.emplace_back([&, i]() {
            // 4. 线程内部开始计时
            auto start = std::chrono::high_resolution_clock::now();
            // 5. 开始循环写日志
            for (int j = 0; j < msg_per_thread; ++j) {
                obj->fatal("%s", msg.c_str());
            }
            // 6. 线程内部结束计时
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> cost = end - start;
            // std::cout << "thread[" << i << "]: "
            //           << "output size: "
            //           << msg_per_thread << " time: "
            //           << cost.count() << "s" << std::endl;
            cost_array[i] = cost.count();
        });
    }
    for (int i = 0; i < thr_count; ++i)
        threads[i].join();
    // 7. 计算时间（在多线程情况下，耗时最长的线程就是我们要的时间，而不是加在一起）
    double max_cost = *std::max_element(cost_array.begin(), cost_array.end());
    double msg_per_sec = msg_count / max_cost;
    double size_per_sec = (msg_count * msg_len) / (max_cost * 1024);
    // std::cout << "message per sec: " << msg_per_sec << std::endl;
    // std::cout << "size per sec: " << size_per_sec / 1024 << "mb" << std::endl;
    return { msg_per_sec, size_per_sec };
}

void make_bench() {
    std::unique_ptr<ffengc_log::loggerBuilder> builder(new ffengc_log::globalLoggerBuilder());
    builder->buildLoggerLevel(ffengc_log::logLevel::value::WARNING);
    builder->buildLoggerName("make_bench");
    builder->buildLoggerType(ffengc_log::loggerType::LOGGER_ASYNC);
    builder->buildEnableUnsafeLoop();
    builder->buildFormatter("%m%n");
    builder->buildSink<ffengc_log::fileSink>("./logfile/sync.log");
    auto logger = builder->build();
    //
    std::vector<double> msg_per_sec_lst;
    std::vector<double> size_per_sec_lst;
    for (int i = 0; i < 10; ++i) {
        auto p = bench("make_bench", 10, 10000000, 100);
        assert((p != std::pair<double, double>()));
        msg_per_sec_lst.push_back(p.first);
        size_per_sec_lst.push_back(p.second);
    }
    double msg_per_sec_mean = std::accumulate(msg_per_sec_lst.begin(), msg_per_sec_lst.end(), 0) / msg_per_sec_lst.size();
    double size_per_sec_mean = std::accumulate(size_per_sec_lst.begin(), size_per_sec_lst.end(), 0) / size_per_sec_lst.size();
    std::cout << "avg message per sec: " << msg_per_sec_mean << std::endl;
    std::cout << "avg size per sec: " << size_per_sec_mean / 1024 << "mb" << std::endl;
}

int main() {
    make_bench();
    return 0;
}