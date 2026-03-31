#pragma once

#include "epoll.h"

class Eventloop
{
private:
    Epoll *ep_;
    /* data */
public:
    Eventloop(/* args */);
    ~Eventloop();

    void run();
    Epoll *ep();
};
