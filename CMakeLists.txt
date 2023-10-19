cmake_minimum_required(VERSION 3.0)
project(Server)

set(CMAKE_CXX_COMPILER g++)  # 指定 C++ 编译器

#如果希望使用单线程运行，可使用下面语句定义宏_SIGLETHREAD_
#add_compile_definitions(_SIGLETHREAD_)

#添加debug信息，使得生成的可执行文件可以调试
set(CMAKE_BUILD_TYPE DEBUG)
#可调试编译选项，这样才能命中断点
add_definitions("-Wall -g")

include_directories(include test)

find_package(jsoncpp REQUIRED)
# 添加可执行文件的目标，包括测试代码源文件和可执行文件的名称
add_executable(Server
  src/message.cc
  src/server.cc
  test/main.cc
  src/abstractHandle.cc
  src/connections.cc
  src/dataProcess.cc
  src/fapplycations.cc
  src/thread.cc
  src/threadPool.cc
  src/timer.cc
  src/util.cc
)

target_link_libraries(Server PRIVATE jsoncpp pthread)







