#include <iostream>
#include <unistd.h> //int getopt(...)
#include <string>

#include "drpcapp.h"

DrpcConfig DrpcApp::m_config;


void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void DrpcApp::Init(int argc, char **argv)
{
    if(argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc, argv, "i:")) != -1) //i:中的冒号:，表示i必须有
    {
        switch(c)
        {
            case 'i':
                config_file = optarg;
                break;
            case '?': // If getopt() does not recognize an option character, it prints an error message to stderr, stores the character in optopt, and returns '?'. 
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            case ':': //getopt() returns ':' instead of '?' to indicate a missing option argument.
                ShowArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    config_file = "../conf/" + config_file;
    //开始加载配置文件 rpcserverip= rpcserverport= zookeeperip= zookeeperport=
    m_config.LoadConfigFile(config_file.c_str()); //静态方法调用静态成员变量
}

DrpcApp& DrpcApp::GetInstance() // 获取唯一的实例对象，static
{
    static DrpcApp app; // static
    return app;
}

DrpcConfig& DrpcApp::GetConfig()
{
    return m_config;
}

//未定义的引用 => 链接错误
//CMakeLists.txt改动了，cmake才会重新生成makefile文件，否则不会，所以每次都要在build文件夹下，rm -rf *