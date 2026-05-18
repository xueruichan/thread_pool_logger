#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "BlockingQueue.h"
//实例化并测试接口（模板类实例化要指明数据类型）
BlockingQueue<int>my_queue;
//生产者逻辑
void Producer(int id)
{
    //消费者先启动，生产者睡眠1s,测试阻塞
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    //生产者产出3个数据
    int data=0;
    for(int i=0;i<3;i++)
    {
        
        data=id*10+i;
        //测试push功能
        my_queue.push(data);
        std::cout<<"生产者"<<id<<" 放入数据："<<data<<std::endl;
    }
}

//消费者逻辑
void Consumer(int id)
{
    int value=0;   //接收数据
    //测试pop功能
    while(my_queue.pop(value))
    {
        std::cout<<"消费者"<<id<<" 取出数据："<<value<<std::endl;
    }
}

int main()
{
    std::thread consumer1(Consumer,1);
    std::thread consumer2(Consumer,2);
    std::thread consumer3(Consumer,3);
    std::thread producer1(Producer,1);
    std::thread producer2(Producer,2);
    std::thread producer3(Producer,3);
    producer1.join();
    producer2.join();
    producer3.join();
    //测试stop()
    my_queue.stop();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    std::cout<<"线程结束未发生死锁"<<std::endl;
    return 0;
}