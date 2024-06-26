#ifndef LOCKQUEUE_H
#define LOCKQUEUE_H

#include <queue>
#include <thread>
#include <mutex> // Linux中的pthread_mutex_t
#include <condition_variable> // Linux中的pthread_condition_t

//异步写日志的日志队列 
template<typename T> //模版代码的函数实现只能写在头文件当中
class LockQueue
{
public:
    //多个worker线程都会写日志queue
    void Push(const T &data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one(); //只有一个线程在队列中取数据
    } //出作用域锁自动释放

    //一个线程读日志queue，写日志文件
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex); //条件变量condition_variable所需的
        while(m_queue.empty())
        {
            //日志队列为空，线程进入wait状态，并释放锁
            m_condvariable.wait(lock);
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }//出作用域锁自动释放
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};

#endif