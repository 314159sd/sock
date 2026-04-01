#include "thread_pool.h"

Thread_pool::Thread_pool(int min, int max) : min_thread(min), max_thread(max),
                                             stop(true), idle_thread(min), cur_thread(min)
{
    m_manager = new std::thread(&Thread_pool::manager, this);

    for (int i = 0; i < min; i++)
    {
        m_workers.emplace_back(std::thread(&Thread_pool::worker, this));
    }
}
Thread_pool::~Thread_pool()
{
    std::cout << get_thread() << std::endl;
    stop.store(true);
    m_cv.notify_all();
    for (auto &i : m_workers)
    {
        if (i.joinable())
        {
            i.join();
        }
    }

    if (m_manager->joinable())
    {
        m_manager->join();
    }
    delete m_manager;
}

void Thread_pool::add_task(std::function<void(void)> task)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    m_tasks.emplace(task);
    locker.unlock();
    m_cv.notify_one();
}

void Thread_pool::manager()
{
    while (!stop.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (idle_thread.load() > (max_thread.load() - min_thread.load())/2 and cur_thread.load() > min_thread.load())
        {
            m_cv.notify_one();
        }
        else if (idle_thread.load() == 0 and cur_thread.load() < max_thread.load())
        {
            m_workers.emplace_back(std::thread(&Thread_pool::worker, this));
            cur_thread++;
            idle_thread++;
        }
    }
}

void Thread_pool::worker()
{

    while (!stop.load())
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        std::function<void()> task = nullptr;

        while (m_tasks.empty() and !stop.load())
        {
            m_cv.wait(locker);
            if (m_tasks.empty())
            {
                cur_thread--;
                // std::cout << "HELLO! \n";
                return;
            }
        }
        task = std::move(m_tasks.front());
        m_tasks.pop();
        locker.unlock();

        if (task)
        {
            idle_thread--;
            task();
            idle_thread++;
            // std::cout << "CURRENT THREAD: " << cur_thread.load() << std::endl;
            // std::cout << "IDLE THREAD: " << cur_thread.load() << std::endl;
        }
    }
}

void func(int x, int y)
{
    int z = x + y;
    std::cout << z << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Thread_pool::Start()
{
    stop.store(false);
}
void Thread_pool::Stop()
{
    stop.store(true);
}