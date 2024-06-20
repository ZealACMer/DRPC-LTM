#ifndef DRPCAPP_H
#define DRPCAPP_H

#include "drpcconfig.h"
#include "drpcchannel.h"
#include "drpccontroller.h"

// drpc框架的基础类（只需要一个，设计成单例模式）,负责框架的一些初始化操作
class DrpcApp
{
public:
    static void Init(int argc, char **argv); //静态的成员方法，只能访问静态的成员变量，所以m_config要定义为static
    static DrpcApp& GetInstance(); // 获取唯一的实例对象，static
    static DrpcConfig& GetConfig();
private:
    static DrpcConfig m_config;
    DrpcApp(){}
    DrpcApp(const DrpcApp&) = delete; // 拷贝构造相关的函数delete掉
    DrpcApp(DrpcApp&&) = delete; // 拷贝构造相关的函数delete掉
};

#endif