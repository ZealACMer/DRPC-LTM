#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "drpcchannel.h"
#include "drpcheader.pb.h"
#include "drpcapp.h"
#include "drpccontroller.h"
#include "zookeeperutil.h"

// header_size + service_name method_name args_size + args

 //所有通过stub代理对象调用的rpc方法，都运行到这里了，统一做rpc方法调用数据的序列化和网络发送
void DrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, 
            const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name(); //service_name
    std::string method_name = method->name(); //method_name

    //获取参数的序列化字符串长度 args_size
    uint32_t args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error!");
        return;
    }

    //定义rpc的请求header
    drpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("serialize rpc header error!");
        return;
    }

    //组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4)); //header_size，按二进制插入，方便按字节数获取数据
    send_rpc_str += rpc_header_str; //rpcheader
    send_rpc_str += args_str; //args

    //打印调试信息
    std::cout << "===========================================" << std::endl;
    std::cout << "header_size:" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
    std::cout << "service_name:" << service_name << std::endl; 
    std::cout << "method_name:" << method_name << std::endl;
    std::cout << "args_str:" << args_str << std::endl; 
    std::cout << "===========================================" << std::endl;

    //这里不需要高并发(ltmuduo网络库)，所以使用tcp编程，完成rpc方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientfd)
    {
        char errtxt[512] = {0}; //组装字符串
        sprintf(errtxt, "create socket error! errno:%d", errno);
        controller->SetFailed("errtxt");
        return;
    }

    ZkClient zkCli;
    zkCli.Start();
    // /UserServiceRpc/Login
    std::string method_path = "/" + service_name + "/" + method_name;
    // 127.0.0.1:8000
    std::string host_data = zkCli.GetData(method_path.c_str());
    if(host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if(idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid!");
        return;
    }
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //连接rpc服务节点
    if(-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        close(clientfd);
        char errtxt[512] = {0}; //组装字符串
        sprintf(errtxt, "connect error! errno:%d", errno);
        controller->SetFailed("errtxt");
        return;
    }

    //发送rpc请求
    if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        close(clientfd);
        char errtxt[512] = {0}; //组装字符串
        sprintf(errtxt, "connect error! errno:%d", errno);
        controller->SetFailed("errtxt");
        return;
    }

    //接收rpc请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        close(clientfd);
        char errtxt[512] = {0}; //组装字符串
        sprintf(errtxt, "connect error! errno:%d", errno);
        controller->SetFailed("errtxt");
        return;
    }

    //反序列化rpc调用的响应数据
    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        close(clientfd);
        char errtxt[512] = {0}; //组装字符串
        sprintf(errtxt, "parse error! response_str:%s", recv_buf);
        controller->SetFailed("errtxt");
        return;
    }
    close(clientfd);
}