/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-12 00:12:48
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-12 01:29:32
 * @FilePath: /oceannes/desktop/include/LogUtil.h
 * @Description: 注释信息
 */
#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <cstring>

enum LogLevel
{
    None,
    Error,
    Info,
    InfoVerbose,
    CpuTrace
};

class LogUtil
{
private:
    /* data */
    LogLevel m_logLevel;
    std::ostream *m_logStream;
    std::ostream *m_cpuTrace;

public:
    LogUtil(/* args */);
    ~LogUtil();

    void setLogStream(std::ostream &stream);
    void setCpuTraceStream(std::ostream &stream);

    std::ostream &getStream();
    std::ostream &getCpuTraceStream();

    LogUtil &setLogLevel(LogLevel level);
    LogLevel getLevel();

    static LogUtil &get();
};

class TeeBuf : public std::streambuf
{
public:
    TeeBuf(std::streambuf *sbuf1, std::streambuf *sbuf2);

private:
    virtual int overflow(int c);
    virtual int sync();

private:
    std::streambuf *m_sbuf1;
    std::streambuf *m_sbuf2;
};

class TeeStream : public std::ostream
{
public:
    TeeStream(std::ostream &o1, std::ostream &o2);

private:
    TeeBuf m_tbuf;
};

#endif