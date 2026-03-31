#pragma once

#include <InetAddress.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>



class Socket
{
    const int fd_;

public:
    Socket(int fd);
    ~Socket();

    int fd() const;
    void bind(const InetAddress &addr);
    void listen(int num = 128);
    int accept(InetAddress &addr);

    void set_reuse_addr(bool flag);
    void set_nodelay(bool flag);
    void set_nonblock(bool flag);

};