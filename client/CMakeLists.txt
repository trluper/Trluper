# 最低CMake版本要求
cmake_minimum_required(VERSION 3.5.1)
 
# 项目名称
project(leetcode)
 
#添加debug信息，使得生成的可执行文件可以调试
set(CMAKE_BUILD_TYPE DEBUG)
#可调试编译选项，这样才能命中断点
add_definitions("-Wall -g")
#使用通配符添加多个源文件
file(GLOB SRC_LIST "src/Server/*.c")
 
#编译选项
add_compile_options(-std=c++11)
 
# 头文件路径
include_directories("client")
 
#链接库
link_libraries(pthread)
 
# 生成可执行的文件
add_executable(client client.cpp)