# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 2.8)
project(drpc)

# 生成debug版本，可以进行gdb调试(如果不加该行，生成的是release版本，没有调试信息)
set(CMAKE_BUILD_TYPE "Debug")

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
# 设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 在src下继续寻找cmakelists生成makefile，用于整个项目的编译
# src包含了drpc框架所有的相关代码
add_subdirectory(src)

# 存放示例业务代码
# example包含了drpc框架使用的示例代码
add_subdirectory(example)