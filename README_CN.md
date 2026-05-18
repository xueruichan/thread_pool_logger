# AsyncLogger & ThreadPool: 高性能 C++11 基础组件库

![C++](https://img.shields.io/badge/C++-11-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 1. 项目简介

本项目是一个基于现代 C++ (C++11) 开发的轻量级后端基础组件库。项目主要包含两个独立且可协同工作的核心模块：**支持泛型任务的并发线程池 (ThreadPool)** 与 **基于阻塞队列的异步日志系统 (AsyncLogger)**。

该系统通过分离前端业务逻辑与后端磁盘 I/O 操作，有效避免了高并发场景下的 I/O 阻塞问题。同时结合 RAII 机制实现了资源的自动管理与程序的安全退出，适用于 C++ 服务端程序（如 Web 服务器、RPC 框架）的底层基础设施建设。

## 2. 核心特性

* **异步日志处理 (Async Logging)**：业务线程负责日志格式化并推入内存队列，独立后台线程负责磁盘 I/O 写入。实现业务逻辑与日志落盘的解耦，保障极低延迟。
* **泛型并发线程池 (Generic Thread Pool)**：利用 C++11 变参模板、`std::future` 与 `std::packaged_task`，支持向线程池提交任意函数签名及带有返回值的任务，支持任务状态监控与结果异步获取。
* **线程安全阻塞队列 (Thread-safe Blocking Queue)**：基于 `std::mutex` 与 `std::condition_variable` 实现高吞吐量的生产者-消费者模型，保障多线程并发写入时的数据一致性。
* **规范化工程架构 (Standardized Architecture)**：严格遵守声明 (`.h`) 与实现 (`.cpp`) 分离原则。引入 CMake 构建系统支持外部构建 (Out-of-source build)，易于集成与扩展。

## 3. 目录结构

```text
thread_pool_logger/
├── CMakeLists.txt        # CMake 自动化构建配置文件
├── include/              # 头文件目录（对外暴露的接口声明）
│   ├── AsyncLogger.h     # 异步日志模块声明
│   ├── BlockingQueue.h   # 线程安全阻塞队列模板实现
│   └── ThreadPool.h      # 线程池模块声明
├── src/                  # 源代码目录（核心模块实现）
│   ├── AsyncLogger.cpp   # 异步日志功能实现
│   ├── ThreadPool.cpp    # 线程池功能实现
│   └── main.cpp          # 模块集成与高并发测试入口
└── demos/                # 独立功能测试代码 (Unit Tests)
```

## 4. 编译与运行指南

本项目采用 CMake 进行构建，建议在 Linux 环境下进行编译。请确保系统已安装 `g++` 编译器与 `cmake` 构建工具。

### 4.1 编译项目

建议使用外部构建以保持源码目录整洁。在项目根目录下依次执行以下命令：

```bash
# 1. 创建并进入独立的构建目录
mkdir build && cd build

# 2. 生成 Makefile
cmake ..

# 3. 执行编译（可根据 CPU 核心数调整 -j 参数，例如 -j4）
make
```

### 4.2 运行测试程序

编译完成后，可执行文件 `server_app` 将生成在 `build` 目录中。为确保日志文件相对路径生成正确，请在项目根目录下执行程序：

```bash
# 退回项目根目录并运行程序
cd ..
./build/server_app
```

运行结束后，系统将在根目录下自动生成 `logs/server.log` 文件。您可以查看该文件以验证多线程并发环境下的日志记录正确性与完整性。