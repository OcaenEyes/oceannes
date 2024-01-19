/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:24:50
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-19 19:52:59
 * @FilePath: /vdesktop/include/Log.h
 * @Description: 注释信息
 */
#pragma once

#include <iostream>

#define LOG(level)                               \
    if (level <= Log::GetInstance().get_level()) \
    Log::GetInstance().get_log_stream() << '[' << __FILE__ << ':' << std::dec << __LINE__ << ']'

#define LOG_CPU                                     \
    if (Log::GetInstance().get_level() == CPUTRACE) \
    Log::GetInstance().get_cpu_trace_stream()

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

    void set_log_stream(std::ostream &stream);
    void set_cpu_trace_stream(std::ostream &stream);

    std::ostream &get_log_stream();
    std::ostream &get_cpu_trace_stream();

    void set_level(LogLevel level);
    LogLevel get_level();
};
