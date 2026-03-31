#pragma once

#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstring>
#include "Channel.h"

class Channel;


class Epoll
{
    static const int max_event{1024};
    int epoll_fd_;
    struct epoll_event events_[max_event];

public:
    Epoll();
    ~Epoll();

    // void addfd(int fd, uint32_t op);
    void updatechannel(Channel *ch);
    // std::vector<epoll_event> loop(int timeout);
    std::vector<Channel *> loop(int timeout);
};
