# set(SRC_LIST userservice.cc ../user.pb.cc)

set(SRC_LIST friendservice.cc ../friend.pb.cc)

# 生成可执行文件
add_executable(provider ${SRC_LIST})

# 链接相应的库文件
target_link_libraries(provider drpc protobuf)