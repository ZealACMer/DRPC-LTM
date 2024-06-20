# 将当前目录下的所有源文件赋给SRC_LIST
aux_source_directory(. SRC_LIST)

# add_library(drpc SHARED ${SRC_LIST})
add_library(drpc STATIC ${SRC_LIST})

target_link_libraries(drpc ltmuduo pthread zookeeper_mt)