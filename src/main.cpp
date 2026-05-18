#include <iostream>
#include <chrono>
#include "ThreadPool.h"
#include "AsyncLogger.h"

int main() {
    std::cout << ">>> 工业级并发基础架构启动 <<<\n";

    {
        // 1. 实例化全局日志系统
        AsyncLogger logger("logs/server.log");
        logger.info("系统日志模块初始化成功。");

        // 2. 实例化线程池 (4个核心)
        ThreadPool pool(4);
        logger.info("线程池初始化成功，核心数：4。");

        // 3. 模拟高并发业务：扔 10 个耗时任务进线程池
        for (int i = 1; i <= 10; ++i) {
            pool.submit([i, &logger]() {
                logger.info("业务任务 " + std::to_string(i) + " 开始处理...");
                
                // 模拟耗时业务操作
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                
                if (i % 3 == 0) {
                    logger.error("业务任务 " + std::to_string(i) + " 触发了预设警告！");
                } else {
                    logger.info("业务任务 " + std::to_string(i) + " 处理完成。");
                }
            });
        }

        // 走到这，主线程瞬间执行完毕，等待局部作用域结束
        logger.info("主线程已将所有任务分发完毕，准备进入安全退出流程。");
        
    } // <--- 关键点：跨越这里，pool 先析构等待所有任务完成，然后 logger 析构把剩余日志刷入磁盘。

    std::cout << ">>> 所有组件安全销毁，请查看 logs/server.log 验收结果。<<<\n";
    return 0;
}