#ifndef OPENFLOW_COMMON_UTILS_H
#define OPENFLOW_COMMON_UTILS_H
#pragma once

namespace common {

class CUtils
{
public:
    static std::string get_program_path();
    static bool block_signal(int signo);
    static std::string generate_uuid();
    static std::string get_local_ip(const char* dev);
};

} // end namespace utils

#endif // OPENFLOW_COMMON_UTILS_H
