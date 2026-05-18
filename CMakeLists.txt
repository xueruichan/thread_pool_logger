#申明cmake最低版本
cmake_minimum_required(VERSION 3.10)

#申明项目名称和语言
project(ThreadPoolLogger CXX)

#设置C++11标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#开启编译警告
add_compile_options(-Wall)

#寻找系统中的线程库（用到了std::thread）,Linux 下底层依赖的是操作系统的 pthread 库
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#指出头文件位置(相当于 g++ 里的 -I include)
#${CMAKE_SOURCE_DIR}：这是一个CMake内置变量,永远指向你最顶层 CMakeLists.txt 文件所在的绝对路径。
include_directories(${CMAKE_SOURCE_DIR}/include)

#将 src 目录下的所有 .cpp源文件找出来，存入名为 SRC_FILES 的变量中
file(GLOB SRC_FILES "src/*.cpp")

#生成可执行文件:add_executable(生成的程序名 依赖的源文件)
add_executable(server_app ${SRC_FILES})

target_link_libraries(server_app Threads::Threads)