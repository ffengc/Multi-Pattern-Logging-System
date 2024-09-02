/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_ASYNC_LOOPER__
#define __YUFC_ASYNC_LOOPER__

#include "buffer.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace ffengc_log {
using functor = std::function<void(buffer&)>;
enum class asyncType {
    ASYNC_SAFE, // 安全状态，表示哈UN冲功能区满了则阻塞，避免资源耗尽的风险
    ASYNC_UNSAFE, // 不考虑资源耗尽，用于压力测试
};
class asyncLooper {
private:
    asyncType __looper_type;
    std::thread __work_thread; // 异步工作器对应的线程 不要取名为 __thread, __thread 是那个用来指定字段每个线程独占的那个关键字
    std::atomic<bool> __stop_signal; // 停止信号
    std::mutex __mtx;
    std::condition_variable __producer_condition;
    std::condition_variable __consumer_condition;
    buffer __producer_buffer; // 生产者缓冲区
    buffer __consumer_buffer; // 消费者缓冲区
public:
    using ptr = std::shared_ptr<asyncLooper>;
    asyncLooper(const functor& callback, const asyncType& looper_type = asyncType::ASYNC_SAFE)
        : __stop_signal(false)
        , __looper_type(looper_type)
        , __callBack(callback)
        , __work_thread(std::thread(&asyncLooper::threadEntry, this)) { }
    ~asyncLooper() {
        stop();
    }
    void stop() {
        __stop_signal = true;
        __consumer_condition.notify_all();
        __work_thread.join();
    }
    void push(const char* data, size_t len) {
        // 1. 无限扩容（非安全，用于压力测试）
        // 2. 固定大小 满了之后需要阻塞
        std::unique_lock<std::mutex> lock(__mtx);
        if (__looper_type == asyncType::ASYNC_SAFE)
            __producer_condition.wait(lock, [&]() { return __producer_buffer.writeableSize() >= len; });
        __producer_buffer.push(data, len);
        __consumer_condition.notify_one(); // 唤醒消费者
    } //
private:
    void threadEntry() {
        while (true) {
            // 1. 判断生产缓冲区是否有数据，有则交换
            {
                std::unique_lock<std::mutex> lock(__mtx);
                __consumer_condition.wait(lock, [&]() { return __stop_signal || !__producer_buffer.empty(); });
                if (__stop_signal && __producer_buffer.empty())
                    break; // 如果生产缓冲区还有数据，那就先不要退出
                __consumer_buffer.swap(__producer_buffer);
                // 4. 唤醒生产者
                if (__looper_type == asyncType::ASYNC_SAFE) // 如果是非安全状态，producer不会阻塞
                    __producer_condition.notify_all();
            }
            // 2. 被唤醒后，对消费缓冲区进行处理
            __callBack(__consumer_buffer);
            // 3. 初始化消费缓冲区
            __consumer_buffer.reset();
        }
    } // 线程的入口函数
private:
    functor __callBack; // 具体对缓冲区数据进行处理的cb函数
};
} // namespace ffengc_log

#endif