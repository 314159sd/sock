#include "Eventloop.h"

Eventloop::Eventloop() : ep_(new Epoll), thread_pool_(new Thread_pool)
{
    thread_pool_->Start();
}

Eventloop::~Eventloop()
{
    delete ep_;
    delete thread_pool_;
}

void Eventloop::run()
{
    while (true)
    {
        auto channels = ep_->loop(-1);
        for (auto &ch : channels)
        {
            ch->handleevent();
        }
    }
}

Epoll *Eventloop::ep()
{
    return ep_;
}

Thread_pool *Eventloop::thread_pool()
{
    return thread_pool_;
}
