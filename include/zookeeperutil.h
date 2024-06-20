#ifndef ZOOKEEPERUTIL_H
#define ZOOKEEPERUTIL_H

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

//封装的zookeeper客户端类
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    //zkclient启动连接zkserver
    void Start();
    //在zkserver上根据指定的path创建znode节点（节点的路径，数据，数据的长度，永久0 or 非永久非0）
    void Create(const char *path, const char *data, int dataLen, int state = 0);
    //根据参数指定的znode节点路径，获取znode节点的值
    std::string GetData(const char *path);
private:
    //zk的客户端句柄，通过该句柄操控zkserver
    zhandle_t *m_zhandle;
};

#endif