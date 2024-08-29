/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#define ONLY_C false
#if ONLY_C

#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// 不定参宏
#define LOG(fmt, ...) printf("[%s, %d]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);
void test1() {
    LOG("hello world\n");
    LOG("hello world: %d\n", 2);
}

// C风格不定参函数
void print_arg_num(int count, ...) {
    va_list ap;
    va_start(ap, count); // 获取指定参数起始地址，这里就是获取count参数之后第一个参数的起始地址
    for (int i = 0; i < count; ++i) {
        int num = va_arg(ap, int); // 这里要告诉 va_arg 是什么类型的数据，这样编译器才知道要在栈上获取具体多少空间
        printf("param[%d]:%d\n", i, num);
    }
    va_end(ap); // 将ap指针置空
}
void test2() {
    print_arg_num(2, 666, 777);
    print_arg_num(5, 1, 2, 3, 4, 5);
}
// 模拟实现 printf
void myprintf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char* res;
    int ret = vasprintf(&res, fmt, ap);
    if (ret != -1) {
        printf(res);
        free(res);
    }
    va_end(ap);
}
void test3() {
    myprintf("hello world: %d\n", 2);
}
#else

#include <iostream>

// C++风格不定参函数
void xprintf() {
    std::cout << std::endl;
}
template <typename T, typename... Args>
void xprintf(const T& v, Args&&... args) {
    std::cout << v << " ";
    if ((sizeof...(args)) > 0)
        xprintf(std::forward<Args>(args)...);
    else
        xprintf();
}
void test4() {
    xprintf("hello");
    xprintf("hello", "world");
    xprintf("hello", "world", 666, std::string("hi"));
}
#endif

int main() {
    test4();
    return 0;
}
