#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "ThreadPool.h"
//创建一个全局锁用于防止打印文字时交错乱码
std::mutex print_mtx;
//模拟实际任务函数
void mock_task(int task_id)
{
    //让线程沉睡，模拟任务的耗时
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    //加锁打印信息
    {
        std::unique_lock<std::mutex>lock (print_mtx);
        //测试每个任务打印 task id 和 thread id
        std::cout<<"任务完成->task_id: "<<task_id<<"  thread id: "<<std::this_thread::get_id()<<std::endl;
    }
}

int main()
{
    {
        std::unique_lock<std::mutex>lcok(print_mtx);
        std::cout<<"正在创建线程池..."<<std::endl;
    }
    //RAII  人为规定了线程池的作用域用于测试是否会调用析构函数销毁线程池
    {
        //测试创建ThreadPool pool(4)
        ThreadPool pool(4);
        {
            std::unique_lock<std::mutex>lcok(print_mtx);
            std::cout<<"线程池创建成功，有4个线程存在"<<std::endl;
            std::cout<<"正在提交10个任务到任务队列"<<std::endl;
        }
        for(int i=1;i<=10;i++)
        {
            //测试使用lambda传递给submit函数并提交到任务队列
            pool.submit([i]()
        {
            mock_task(i);
        });
        }
        {
            std::unique_lock<std::mutex>lcok(print_mtx);
            std::cout<<"所有任务已经成功提交到任务队列(BlockingQueue)"<<std::endl;
        }
    }
    std::cout<<"测试完成，线程池安全退出，并未发生死锁"<<std::endl;
    return 0;
}