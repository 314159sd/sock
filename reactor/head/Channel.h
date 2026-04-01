#pragma once

#include "InetAddress.h"
#include <functional>
#include "Socket.h"
#include "thread_pool.h"
#include <sys/epoll.h>

class Epoll;
class Eventloop;
class Channel
{
private:
    int fd_;
    Epoll *ep_;
    Eventloop *loop_;
    bool inepoll_{false};
    uint32_t event_;
    uint32_t revent_;
    std::function<void()> readcallback_;

    InetAddress *ac_addr = new InetAddress("0.0.0.0", 8080);

public:
    Channel(int fd, Epoll *ep);
    ~Channel();
    void enableIN();
    void enableOUT();

    void setET();
    void setRevent(uint32_t revent);
    void setinepoll(bool flag);

    bool inepoll();
    int fd();
    uint32_t event();
    uint32_t revent();

    void handleevent();
    void newconnect(Socket &sock);
    void onmessage();
    void setcallback(std::function<void()> func);
};
