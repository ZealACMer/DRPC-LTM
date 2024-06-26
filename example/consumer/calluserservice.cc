#include <iostream>
#include "drpcapp.h"
#include "user.pb.h"
#include "drpcchannel.h"

int main(int argc, char **argv)
{
    //整个程序启动后，使用该框架提供的rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    DrpcApp::Init(argc, argv);

    //演示调用远程发布的rpc方法Login
    testProto::UserServiceRpc_Stub stub(new DrpcChannel());

    //rpc方法的请求参数
    testProto::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    
    //rpc方法的响应
    testProto::LoginResponse response;

    //发起rpc方法的调用 => 同步的rpc调用过程 DrpcChannel::callMethod
    stub.Login(nullptr, &request, &response, nullptr); //RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送

    //一次rpc调用完成，读调用的结果
    if(0 == response.result().errcode())
    {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    //调用远程发布的rpc方法Register
    testProto::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("666666");
    testProto::RegisterResponse rsp;

    //以同步的方式发起rpc调用请求，等待返回结果
    stub.Register(nullptr, &req, &rsp, nullptr);

    //一次rpc调用完成，读调用的结果
    if(0 == response.result().errcode())
    {
        std::cout << "rpc register response success: " << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error: " << response.result().errmsg() << std::endl;
    }
    

    return 0;
}