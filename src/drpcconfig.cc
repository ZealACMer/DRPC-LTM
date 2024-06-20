#include <iostream>
#include <string>

#include "drpcconfig.h"

// 负责加载解析配置文件
void DrpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if(nullptr == pf)
    {
        std::cout << config_file << " is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }


    while(!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf); //读取一行
        std::string read_buf(buf); //将buf转为字符串string类型，方便处理

        // 判断#的注释
        if(read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        // 解析配置项
        int idx = read_buf.find('=');
        if(idx == -1)
        {
            // 配置项不合法
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0, idx);
        Trim(key); //去掉key前后的空格

        // rpcserverip=127.0.0.1\n
        int endidx = read_buf.find('\n', idx); //从idx向后寻找\n字符
        value = read_buf.substr(idx + 1, endidx - idx - 1);
        Trim(value);
        m_configMap.insert({key, value});

    }
}

// 查询配置项信息
std::string DrpcConfig::Load(const std::string &key) const
{
    //错误 => return m_configMap[key]; 具有补作用，key如果存在，则返回相应的value值，key如果不存在，则新建一个键值对
    auto it = m_configMap.find(key);
    if(it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}

//去掉字符串前后的空格
void DrpcConfig::Trim(std::string &src_buf) //因为参数要改变，所以使用了引用
{
    // 去掉字符串前面多余的空格
    int idx = src_buf.find_first_not_of(' '); //从前往后找第一个不是空格的字符
    if(idx != -1) //找到了符合条件的字符
    {
        src_buf = src_buf.substr(idx, src_buf.size() - idx); //第二个参数为字符串截取的长度
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' '); //从后往前找第一个不是空格的字符
    if(idx != -1) //找到了符合条件的字符
    {
        src_buf = src_buf.substr(0, idx + 1);
    }
}