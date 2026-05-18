# AsyncLogger & ThreadPool: High-Performance C++11 Basic Components Library

![C++](https://img.shields.io/badge/C++-11-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 1. Project Introduction

This project is a lightweight backend basic components library developed based on modern C++ (C++11). The project mainly contains two independent yet collaborative core modules: a **concurrent thread pool supporting generic tasks (ThreadPool)** and an **asynchronous logging system based on a blocking queue (AsyncLogger)**.

By separating front-end business logic from back-end disk I/O operations, this system effectively avoids I/O blocking issues in high-concurrency scenarios. Combined with the RAII mechanism, it achieves automatic resource management and safe program termination. It is highly suitable for building the underlying infrastructure of C++ server programs (such as Web servers, RPC frameworks).

## 2. Core Features

* **Async Logging**: Business threads are responsible for formatting logs and pushing them into an in-memory queue, while a dedicated background thread handles disk I/O writing. This decouples business logic from log flushing, ensuring ultra-low latency.
* **Generic Thread Pool**: Utilizing C++11 variadic templates, `std::future`, and `std::packaged_task`, it supports submitting tasks with arbitrary function signatures and return values to the thread pool, allowing for task state monitoring and asynchronous result retrieval.
* **Thread-safe Blocking Queue**: Implements a high-throughput producer-consumer model based on `std::mutex` and `std::condition_variable`, ensuring data consistency during concurrent writes by multiple threads.
* **Standardized Architecture**: Strictly adheres to the separation of declarations (`.h`) and implementations (`.cpp`). Introduces the CMake build system to support out-of-source builds, making it easy to integrate and scale.

## 3. Directory Structure

```text
thread_pool_logger/
©À©¤©¤ CMakeLists.txt        # CMake automated build configuration file
©À©¤©¤ include/              # Header files directory (exposed interface declarations)
©¦   ©À©¤©¤ AsyncLogger.h     # Async logger module declaration
©¦   ©À©¤©¤ BlockingQueue.h   # Thread-safe blocking queue template implementation
©¦   ©¸©¤©¤ ThreadPool.h      # Thread pool module declaration
©À©¤©¤ src/                  # Source code directory (core module implementations)
©¦   ©À©¤©¤ AsyncLogger.cpp   # Async logger function implementation
©¦   ©À©¤©¤ ThreadPool.cpp    # Thread pool function implementation
©¦   ©¸©¤©¤ main.cpp          # Module integration and high-concurrency testing entry
©¸©¤©¤ demos/                # Independent function testing code (Unit Tests)
```

## 4. Build and Run Guide

This project uses CMake for building and is recommended to be compiled in a Linux environment. Please ensure that the `g++` compiler and `cmake` build tools are installed on your system.

### 4.1 Build the Project

Out-of-source builds are recommended to keep the source directory clean. Execute the following commands in sequence in the project root directory:

```bash
# 1. Create and enter an independent build directory
mkdir build && cd build

# 2. Generate the Makefile
cmake ..

# 3. Execute compilation (adjust the -j parameter based on your CPU cores, e.g., -j4)
make
```

### 4.2 Run the Test Program

After compilation, the executable file `server_app` will be generated in the `build` directory. To ensure the relative path for the log file is generated correctly, please execute the program from the project root directory:

```bash
# Return to the project root directory and run the program
cd ..
./build/server_app
```

After execution, the system will automatically generate a `logs/server.log` file in the root directory. You can check this file to verify the correctness and completeness of the log records in a multi-threaded concurrent environment.