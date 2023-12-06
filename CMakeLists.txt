cmake_minimum_required(VERSION 3.0)
#project(Server)
project(Trluper)

set(CMAKE_CXX_COMPILER g++)  # 指定 C++ 编译器
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

#如果希望使用单线程运行，可使用下面语句定义宏_SIGLETHREAD_
#add_compile_definitions(_SIGLETHREAD_)

#添加debug信息，使得生成的可执行文件可以调试
#set(CMAKE_BUILD_TYPE DEBUG)
#可调试编译选项，这样才能命中断点
#add_definitions("-Wall -g")

find_package(jsoncpp REQUIRED)
if(jsoncpp_FOUND)
    include_directories(${jsoncpp_INCLUDE_DIR})
endif()

find_package(yaml-cpp REQUIRED)
if(yaml-cpp_FOUND)
    include_directories(${yaml-cpp_INCLUDE_DIR})
endif()



# 添加可执行文件的目标，包括测试代码源文件和可执行文件的名称
set(SOURCE_DIR
  src/message.cc
  src/server.cc
  src/abstractHandle.cc
  src/connections.cc
  src/dataProcess.cc
  src/fapplycations.cc
  src/thread.cc
  src/threadPool.cc
  src/timer.cc
  src/util.cc
  src/log.cc
  src/mysqlpool.cc
)

add_library(Trluper SHARED ${SOURCE_DIR})
include_directories(include)

set(LIBS
    Trluper
    jsoncpp
    pthread
    yaml-cpp
    mysqlclient
    )


add_executable(Server test/Server.cc)
target_include_directories(Server PUBLIC test)
target_link_libraries(Server ${LIBS}) 

add_executable(timer test/timer_test.cc)
target_include_directories(timer PUBLIC test)
target_link_libraries(timer ${LIBS})

add_executable(log test/test_log.cc)
target_include_directories(log PUBLIC test)
target_link_libraries(log ${LIBS})

add_executable(coroutine test/test_coroutine.cc)
target_include_directories(coroutine PUBLIC test)
target_link_libraries(coroutine ${LIBS})

add_executable(mysql test/test_mysqlpool.cc)
target_include_directories(mysql PUBLIC test)
target_link_libraries(mysql ${LIBS})