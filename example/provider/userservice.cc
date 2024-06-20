#include <iostream>
#include <string>
#include "user.pb.h"
#include "drpcapp.h" //cmake制定了头文件的搜索路径，不必使用相对路径
#include "drpcprovider.h"

/*
UserService原来是一个本地服务，提供了两个进程内的本地方法Login和GetFriendList
*/

class UserService : public testProto::UserServiceRpc //使用在RPC服务发布端(RPC服务的提供者)
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id:" << id << "name:" << name << " pwd:" << pwd << std::endl;
        return true;
    }

    //重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    /*
    1. 调用者 ==> Login(LoginRequest) ==> ltmuduo ==> 被调用者
    2. 被调用者 ==> Login(LoginRequest) ==> 交到下面重写的这个Login方法上
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::testProto::LoginRequest* request,
                       ::testProto::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        //做本地业务
        bool login_result = Login(name, pwd); 

        //把响应写入response 包括错误码、错误消息、返回值
        testProto::ResultCode *code = response->mutable_result();
        code->set_errcode(0); //code->set_errcode(1); 
        code->set_errmsg(""); //code->set_errmsg("Login do error!");
        response->set_success(login_result);

        //执行回调操作 执行响应对象数据的序列化和网络发送(都是由框架来完成的)
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                    const ::testProto::RegisterRequest* request,
                    ::testProto::RegisterResponse* response,
                    ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id(); //框架已经在网络上接收到请求数据，并将其反序列化为RegisterRequest对象，接下来获取相应数据
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd); //做本地业务

        response->mutable_result()->set_errcode(0); //对response填写响应值
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);

        done->Run(); //执行done的回调
    }
};

int main(int argc, char **argv)
{
    //调用框架的初始化操作
    DrpcApp::Init(argc, argv);

    //provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    //provider作为服务提供方，有很多服务请求，所以要求高并发，使用ltmuduo网络库实现
    DrpcProvider provider;
    provider.NotifyService(new UserService());

    //启动一个rpc服务发布节点，Run()之后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}