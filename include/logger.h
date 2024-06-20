#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include "lockqueue.h"

enum LogLevel
{
    INFO, //普通信息
    ERROR, //错误信息
};

//Drpc框架提供的日志系统
class Logger
{
public:
    //获取日志的单例
    static Logger& GetInstance();
    //设置日志级别
    void SetLogLevel(LogLevel level);
    //写日志
    void Log(std::string msg);
private:
    int m_loglevel; //记录日志级别
    LockQueue<std::string> m_lckQue; //日志缓冲队列

    Logger(); //为了使用单例模式，将默认构造函数设为私有，拷贝构造相关的函数都delete掉，标识相应的成员函数不可用
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

//定义宏 LOG_INFO("xxx %d %s", )
//snprintf(缓冲区，缓冲区的大小，格式化字符串，可变参数的列表)
#define LOG_INFO(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    }while (0);

#define LOG_ERR(logmsgformat, ...) \
    do \
    {  \
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.Log(c); \
    }while (0);

#endif