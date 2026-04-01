#pragma once

#include "epoll.h"
#include "thread_pool.h"

class Eventloop
{
private:
    Epoll *ep_;
    Thread_pool *thread_pool_;

    /* data */
public:
    Eventloop(/* args */);
    ~Eventloop();

    void run();
    Epoll *ep();
    Thread_pool *thread_pool();
};
