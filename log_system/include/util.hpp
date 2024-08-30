/*
 * Write by Yufc
 * See https://github.com/ffengc/Multi-Pattern-Logging-System
 * please cite my project link: https://github.com/ffengc/Multi-Pattern-Logging-System when you use this code
 */

#ifndef __YUFC_UTIL__
#define __YUFC_UTIL__

#include <ctime>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

namespace ffengc_log {
namespace util {
    class Date {
    public:
        static size_t now() { return (size_t)time(nullptr); }
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
                std::string parent_dir = path_name.substr(0, pos + 1);
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

#endif