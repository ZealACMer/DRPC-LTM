#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "drpcapp.h"
#include "drpcprovider.h"
#include "logger.h"

//可以在一个服务器上，发布多个服务及其对应的方法
class FriendService : public testProto::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {
        std::cout << "do GetFriendList service! userid:" << userid << std::endl;;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("liu hong");
        vec.push_back("wang shuo");
        return vec;
    }

    //重写基类方法，被框架调用
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::testProto::GetFriendListRequest* request,
                       ::testProto::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for(std::string &name : friendsList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)
{
    LOG_INFO("first log messge!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);

    //调用框架的初始化操作
    DrpcApp::Init(argc, argv);

    //provider是一个rpc网络服务对象，把FriendService对象发布到rpc节点上
    DrpcProvider provider;
    provider.NotifyService(new FriendService());

    //启动一个rpc服务发布节点，Run()之后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}

//类中都是纯虚函数 => 抽象类 =>不能被实例化
//需要创建一个类，来继承此抽象类，并且重写其中的纯虚函数，然后进行实例化