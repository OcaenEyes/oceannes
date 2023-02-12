/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-12 00:13:38
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-12 15:43:02
 * @FilePath: /oceannes/desktop/src/LogUtil.cpp
 * @Description: 注释信息
 */
#include "LogUtil.h"

LogUtil::LogUtil(/* args */)
{
}

LogUtil::~LogUtil()
{
}

LogUtil &LogUtil::get()
{
    static LogUtil instance;
    return instance;
}

std::ostream &LogUtil::getCpuTraceStream()
{
    return *m_cpuTrace;
}

std::ostream &LogUtil::getLogStream()
{
    return *m_logStream;
}

void LogUtil::setLogStream(std::ostream &stream)
{
    m_logStream = &stream;
}

void LogUtil::setCpuTraceStream(std::ostream &stream)
{
    m_cpuTrace = &stream;
}

LogUtil &LogUtil::setLogLevel(LogLevel level)
{
    m_logLevel = level;
    return *this;
}

LogLevel LogUtil::getLevel()
{
    return m_logLevel;
}

TeeBuf::TeeBuf(std::streambuf *sbuf1, std::streambuf *sbuf2)
{
    m_sbuf1 = sbuf1;
    m_sbuf2 = sbuf2;
}

int TeeBuf::overflow(int c)
{
    if (c == EOF)
    {
        return !EOF;
    }
    else
    {
        int const r1 = m_sbuf1->sputc(c);
        int const r2 = m_sbuf2->sputc(c);
        return r1 == EOF || r2 == EOF ? EOF : c;
    }
}

int TeeBuf::sync()
{
    int const r1 = m_sbuf1->pubsync();
    int const r2 = m_sbuf2->pubsync();

    return r1 == 0 && r2 == 0 ? 0 : -1;
}

TeeStream::TeeStream(std::ostream &o1, std::ostream &o2) : std::ostream(&m_tbuf), m_tbuf(o1.rdbuf(), o2.rdbuf()) {}
