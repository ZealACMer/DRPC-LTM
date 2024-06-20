# DRPC-LTM (Distributed Remote procedure call Based on Lightweighted Tcp Muduo)
C++分布式网络通信框架

## 项目背景
随着软件规模的扩大，传统的编程模式从开始的“单机服务器”，发展到“集群服务器”，进而演变成“分布式服务”的模式，催生了不同机器上运行的服务，或者同一台机器上不同进程中运行的服务(主要指的是docker虚拟化环境)之间进行远程过程调用(rpc, remote procedure call)的需求，该分布式网络通信框架项目就是针对此场景产生的。

让我们先来回顾一下单机服务器、集群服务器存在的问题，以及分布式服务解决了上述两种软件部署模式的哪些痛点：
- 单机服务器存在的问题
  1. 受限于单机服务器的硬件资源，其所能承受的用户并发访问量有限。
  2. 单机服务器包含了所有的服务模块，其中任意模块的更改，会导致整个项目代码的重新编译及部署。
  3. 单机服务器中不同模块的类型有所不同，有的是I/O密集型，有的是CPU密集型，但是因为各个模块是一个整体，无法针对模块类型做个性化的硬件资源配置。
- 集群服务器对单机服务器的改进及存在的问题：
  1. 集群服务器由多个单机服务器组成，通过负载均衡算法，可以将用户请求路由到不同的单机服务器进行处理，可以提高用户的并发访问量。
  2. 运行集群的模式，对单机服务器进行扩展非常简单，如果是针对临时的紧急情况，需要扩充系统的并发访问量，可以迅速得以实现。
  3. 由于集群服务器中的各个单机服务器依然包含所有的服务模块，如果任何的模块进行了更改，项目代码依然需要进行重新编译，并多次部署在不同的机器上。
- 分布式服务的特点：
  1. 将服务模块部署在不同的机器上，所有的机器一起对外提供完整的服务。
  2. 可以根据不同模块的特点，提供不同的硬件资源支持，比如对I/O密集型的模块，提供更优的带宽资源，对CPU密集型的模块，提供更好的CPU和内存支持。
  3. 针对访问量比较大的模块，可以根据情况以集群部署的方式，对其进行扩充，以期为用户提供更好的使用体验。

## DRPC-LTM分布式网络通信框架（流程简介）
<img width="826" alt="DRPC-LTM" src="https://github.com/ZealACMer/DRPC-LTM/assets/16794553/a7370b99-00dc-4808-a380-f88a86a62165">


上图中绿色方框内的内容，即为本分布式网络通信框架实现的核心功能。
                        
## 环境配置
#### Protobuf的安装(本项目使用v3.24.0)
Protobuf是Google开发的一种用于跨平台信息交互的数据格式，支持C++、Go、Python、Java等语言，其在更节省网络带宽和存储空间的同时，传输数据的速度要优于xml和json。(Protobuf采用紧凑的二进制存储，xml和json采用文本存储，且xml需要存储标签信息，json需要存储键值对信息)
  1. 安装依赖工具列表：autoconf automake libtool curl make g++ unzip
  2. github源代码下载地址：[https://github.com/google/protobuf](https://github.com/google/protobuf)
  3. 解压压缩包：unzip protobuf-master.zip
  4. 进入解压后的文件夹：cd protobuf-master
  5. 自动生成configure配置文件：./autogen.sh
  6. 配置环境：./configure
  7. 编译源代码：make
  8. 安装: sudo make install
  9. 刷新动态库：sudo ldconfig
如果在终端输入protoc，可以输出相关信息，则说明安装成功。

#### LTMuduo网络库的安装
请参照[https://github.com/ZealACMer/LTMuduo](https://github.com/ZealACMer/LTMuduo)

#### Zookeeper的安装(本项目使用v3.7.2)
请参照[https://zookeeper.apache.org/](https://zookeeper.apache.org/)

## 构建
本项目提供了一键构建脚本autobuild.sh，可一键进行编译及构建。

## 使用示例
本项目的主要文件夹简介：bin（用于存放可执行文件及配置文件），build（用于存放执行编译和构建时生成的中间文件），conf（存放配置文件），example（存放本框架的使用示例文件），lib（存放本项目生成的静态库文件及将框架集成在其他项目中时需要的头文件），src（存放框架的源文件）。

example中的子文件夹provider提供了远程服务发布方的编程示例，另一个子文件夹consumer提供了远程服务调用方的编程示例。通过autobuild.sh脚本构建项目完成后，在bin目录里面，会有可执行文件provider和consumer，如下图所示：
![DRPC-LTM_2](https://github.com/ZealACMer/DRPC-LTM/assets/16794553/137707e7-819a-45f9-898f-515336dc69f2)

在服务器上启动zookeeper之后，在bin目录下，执行命令./provider -i test.conf先行启动服务提供方的服务，随后执行命令./consumer -i test.conf，如果服务申请端返回"rpc GetFriendList response success!"，则说明运行成功。

## 如果在自己的项目中使用DRPC-LTM分布式网络通信框架
1. 确保服务请求方和服务提供方获取了一致的.proto文件(对发送请求参数和响应参数达成一致)，并使用protoc 文件名.proto --cpp_out=./命令生成相应的.pb.cc文件和.pb.h文件。
2. 服务请求方的配置文件中需包含zookeeper的ip地址和端口号；服务提供方的配置文件中除了包含zookeeper的ip地址和端口号信息之外，还需要配置LTMuduo网络库提供服务的ip地址和端口号。
3. 确保服务请求方和服务提供方所在的主机都正确安装配置了zookeeper和protobuf(服务提供方要额外安装LTMuduo网络库)。
4. 请确保zookeeper在所在的服务器上正确运行。
5. 在各个服务请求方和服务提供方所在的主机中，将本项目中生成的libdrpc.a静态库文件放在usr/local/lib目录中，将lib文件夹下include中的头文件放在usr/local/include目录中。
6. 按照example中的示例编写服务请求方源文件及服务提供方源文件。对于服务请求方的源文件而言，仅需改动申请参数，方法名称，以及接收到响应信息后的处理逻辑；对于服务提供方的源文件而言，仅需修改本地方法的处理逻辑。
7. 在生成服务请求方及服务提供方的可执行文件时，连接libdrpc.a和libprotobuf.a。先后运行服务提供方的服务，和服务请求方的服务，就可以完成远程服务的调用。

