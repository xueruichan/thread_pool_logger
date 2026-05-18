#include <iostream>
#include <thread>
#include <vector>
#include <string>

// 引入异步日志核心类
#include "AsyncLogger.h"
//.......

int main() {    
    std::cout << ">>> 异步日志系统集成测试启动 <<<\n";

    // ---------------------------------------------------------
    // 【目标 4 核心设计】：生命周期管理与安全退出
    // 使用局部作用域包裹 logger 对象。当程序执行到右侧大括号 '}' 时，
    // 将触发 ~AsyncLogger() 析构函数，验证后台线程是否能将队列清空并安全关闭文件。
    // ---------------------------------------------------------
    {
        // 实例化日志模块，将日志输出至指定目录
        AsyncLogger logger("logs/server.log");

        // ---------------------------------------------------------
        // 【目标 1】：单线程写入测试 (验证基础接口 info/error)
        // ---------------------------------------------------------
        std::cout << "[阶段 1] 验证单线程写入逻辑...\n";
        
        logger.info("System initialization started.");
        logger.info("Loading configuration files from /etc/server.conf...");
        logger.error("Failed to load module 'NetworkCache': missing dependencies.");


        // ---------------------------------------------------------
        // 【目标 2 & 3】：多线程并发写入测试 (验证线程安全与线程ID获取)
        // ---------------------------------------------------------
        std::cout << "[阶段 2] 验证多线程并发竞争写入...\n";
        
        std::vector<std::thread> workers;

        // 创建 4 个工作线程并发执行
        for (int i = 1; i <= 4; ++i) {
            workers.emplace_back([i, &logger]() {
                
                // 模拟真实的业务处理流程日志
                logger.info("Worker task " + std::to_string(i) + " started processing client request.");
                logger.info("Worker task " + std::to_string(i) + " successfully committed database transaction.");
                
            });
        }

        // 使用传统的下标索引 for 循环，等待所有工作线程执行完毕
        for (size_t i = 0; i < workers.size(); ++i) {
            if (workers[i].joinable()) {
                workers[i].join();
            }
        }

        // 业务线程已全部同步完毕
        logger.info("All worker threads have joined. System shutting down gracefully.");

    } // <--- 关键触发点：logger 对象在此处销毁，唤醒并等待日志线程收尾。

    // ---------------------------------------------------------
    // 【目标 4】：验证进程存活状态
    // ---------------------------------------------------------
    // 若能正常打印以下信息，说明系统在经历了高并发写入后，成功实现了资源的安全释放，未产生死锁或段错误。
    std::cout << "\n系统退出：日志后台线程已安全终止。\n";
    std::cout << "请查阅 'logs/server.log' 文件核对最终日志格式及完整性。\n";

    return 0;
}