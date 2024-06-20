#ifndef DRPCCHANNEL_H
#define DRPCCHANNEL_H

//#pragma once

#include "google/protobuf/service.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

class DrpcChannel : public google::protobuf::RpcChannel
{
public:
    //所有通过stub代理对象调用的rpc方法，都运行到了这里，统一做rpc方法调用数据的序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, 
                const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done);
};

#endif