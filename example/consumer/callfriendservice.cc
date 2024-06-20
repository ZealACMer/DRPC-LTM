#include <iostream>
#include "drpcapp.h"
#include "friend.pb.h"


int main(int argc, char ** argv)
{
    //整个程序启动以后，使用该框架的rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    DrpcApp::Init(argc, argv);

    //调用远程发布的rpc方法GetFriendsList
    testProto::FriendServiceRpc_Stub stub(new DrpcChannel());
    
    //rpc方法的请求参数
    testProto::GetFriendListRequest request;
    request.set_userid(1000);

    //rpc方法的响应
    testProto::GetFriendListResponse response;

    //发起rpc方法的调用 同步的rpc调用过程 DrpcChannel::callMethod
    DrpcController controller;
    stub.GetFriendList(&controller, &request, &response, nullptr); //RpcChannel->RpcChannel::callMethod

    //一次rpc调用完成，读取调用的结果
    if(controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if(0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendList response success!" << std::endl;
            int size = response.friends_size();
            for(int i = 0; i < size; ++i)
            {
                std::cout << "index:" << (i + 1) << " name:" << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendList response error:" << response.result().errmsg() << std::endl;
        }
    }
    return 0;
}