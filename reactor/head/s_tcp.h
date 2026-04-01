#pragma once


#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <Socket.h>
#include <epoll.h>
#include <Eventloop.h>
#include <Channel.h>


class TCP
{
    int sock_fd;
    bool reuse_addr;
    bool no_delay;
    bool no_block;

public:
    TCP(bool reuse_addr = true, bool no_delay = true, bool no_block = true);
    ~TCP();
};
