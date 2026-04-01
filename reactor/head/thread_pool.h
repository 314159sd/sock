#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <utility>

class Thread_pool
{

    std::thread *m_manager;
    std::atomic<int> min_thread;
    std::atomic<int> max_thread;
    std::atomic<int> cur_thread;
    std::atomic<int> idle_thread;
    std::atomic<bool> stop;

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    void manager(void);
    void worker(void);
    

public:
    Thread_pool(int min = 2, int max = std::thread::hardware_concurrency());
    ~Thread_pool();
    void Start();
    void Stop();
    void add_task(std::function<void(void)> task);
    int get_thread(){return cur_thread.load();}

};