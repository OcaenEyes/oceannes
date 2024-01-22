/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:24:50
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-22 20:05:46
 * @FilePath: /vdesktop/include/Log.h
 * @Description: 注释信息
 */
#pragma once

#include <iostream>

#define LOG(level)                               \
    if (level <= Log::GetInstance().get_level()) \
    Log::GetInstance().get_log_stream() << '[' << __FILE__ << ':' << std::dec << __LINE__ << ']'

#define LOG_INFO(logformat, ...)                        \
    do                                                  \
    {                                                   \
        Log &log = Log::GetInstance();                  \
        log.set_log_level(INFO);                        \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        log.PLog(c);                                    \
    } while {0};

enum LogLevel
{
    NONE,
    ERROR,
    INFO,
    INFOVERBOSE,
    CPUTRACE
};

class Log
{
private:
    LogLevel m_log_level;
    std::ostream *m_log_stream;
    std::ostream *m_cpu_trace_stream;

    Log(/* args */);
    ~Log();
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;

public:
    static Log &GetInstance();

    void PLog(std::string msg);

    void set_log_stream(std::ostream &stream);
    void set_cpu_trace_stream(std::ostream &stream);

    std::ostream &get_log_stream();
    std::ostream &get_cpu_trace_stream();

    void set_level(LogLevel level);
    LogLevel get_level();
};
