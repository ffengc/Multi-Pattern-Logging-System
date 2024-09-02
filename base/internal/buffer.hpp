/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_BUFFER__
#define __YUFC_BUFFER__

#include "util.hpp"
#include <algorithm>
#include <assert.h>
#include <vector>

namespace ffengc_log {
#define DEFAULT_BUFFER_SIZE (1 * 1024 * 1024)
#define THRESHOLD_BUFFER_SIZE (8 * 1024 * 1024)
#define LINEAR_INCREMENT_BUFFER_SIZE (1 * 1024 * 1024)
class buffer {
public:
    std::vector<char> __buffer;
    size_t __read_idx;
    size_t __write_idx; //
public:
    buffer()
        : __buffer(DEFAULT_BUFFER_SIZE)
        , __write_idx(0)
        , __read_idx(0) { }
    void push(const char* data, size_t len) {
        // 0. 缓冲区空间不够的情况: 1. 扩容, 2.阻塞/返回false
        // if(len > writeableSize()) return;
        // assert(len <= writeableSize());
        alloc(len);
        // 1. 将数据拷贝进缓冲区
        std::copy(data, data + len, &__buffer[__write_idx]);
        // 2. 将写入位置向后偏移
        moveWriter(len);
    } // 向缓冲区写入数据
    size_t writeableSize() {
        // 对于扩容思路来说，这个接口没啥用，因为总是可写的，因此这个接口仅仅针对只有固定大小buffer提供的
        return (__buffer.size() - __write_idx);
    } // 返回可写数据的长度（buffer剩余空间）
    const char* begin() {
        return &__buffer[__read_idx];
    } // 返回可读数据的起始地址
    size_t readableSize() {
        return __write_idx - __read_idx;
    } // 返回可读数据的长度
    void moveWriter(size_t len) {
        assert((len + __write_idx) <= __buffer.size());
        __write_idx += len;
    }
    void moveReader(size_t len) {
        assert(len <= readableSize());
        __read_idx += len;
    }
    void reset() {
        __read_idx = __write_idx = 0;
        // 不需要释放空间
    } // 重置读写位置
    void swap(buffer& bf) {
        __buffer.swap(bf.__buffer);
        std::swap(__read_idx, bf.__read_idx);
        std::swap(__write_idx, bf.__write_idx);
    }
    bool empty() {
        return __read_idx == __write_idx;
    } //
private:
    // 扩容
    void alloc(size_t len) {
        if (len <= writeableSize())
            return; // 不需要扩容
        size_t new_size = 0;
        if (__buffer.size() < THRESHOLD_BUFFER_SIZE)
            new_size = __buffer.size() * 2 + len; // 翻倍增长
        else
            new_size = __buffer.size() + LINEAR_INCREMENT_BUFFER_SIZE; // 线性增长
        __buffer.resize(new_size);
    }
};
} // namespace ffengc_log

#endif
