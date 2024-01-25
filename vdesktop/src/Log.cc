/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:20
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-25 21:09:03
 * @FilePath: \vdesktop\src\Log.cc
 * @Description: 注释信息
 */
#include "Log.h"
#include <ctime>

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

void Log::PLog(std::string msg)
{
    time_t now = time(nullptr); // 获取当前时间
    tm *now_tm = localtime(&now);

    char time_buf[128];
    sprintf(time_buf, "%d-%d-%d =>[%s] ",
            now_tm->tm_hour,
            now_tm->tm_min,
            now_tm->tm_sec,
            m_log_level == INFO ? "INFO" : "ERROR"); // 每行日志的时分秒

    msg.insert(0, time_buf); // 在日志msg前面插入时分秒
    msg.append("\n");        // 在末尾增加换行符
    std::cout << msg;
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
