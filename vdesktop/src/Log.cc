/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:20
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-19 19:31:52
 * @FilePath: /vdesktop/src/Log.cc
 * @Description: 注释信息
 */
#include "Log.h"

Log::Log() : m_log_level(INFO)
{
}

Log::~Log()
{
}

Log &Log::GetInstance()
{
    static Log instance;
    return instance;
}

void Log::set_log_stream(std::ostream &stream)
{
    m_log_stream = &stream;
}

void Log::set_cpu_trace_stream(std::ostream &stream)
{
    m_cpu_trace_stream = &stream;
}

std::ostream &Log::get_log_stream()
{
    return *m_log_stream;
}

std::ostream &Log::get_cpu_trace_stream()
{
    return *m_cpu_trace_stream;
}

void Log::set_level(LogLevel level)
{
    m_log_level = level;
}

LogLevel Log::get_level()
{
    return m_log_level;
}
