#ifndef DRPCCONFIG_H
#define DRPCCONFIG_H

#include <unordered_map> //读配置文件需要消耗磁盘I/O，为了提升读取效率，将读取的结果存在哈希表中，提供给各个模块使用
#include <string>

// rpcserverip rpcserverport zookeeperip zookeeperport
// 框架读取配置文件类
class DrpcConfig
{
public:
    //负责加载解析配置文件
    void LoadConfigFile(const char *config_file);

    //查询配置项信息
    std::string Load(const std::string &key) const;

private:
    std::unordered_map<std::string, std::string> m_configMap;
    //去掉字符串前后的空格
    void Trim(std::string &src_buf); //因为参数要改变，所以使用了引用
};

#endif