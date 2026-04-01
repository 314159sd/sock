#include "Channel.h"
#include <epoll.h>
#include <Eventloop.h>
/*
class Channel
{
private:
    int fd_;
    Epoll *ep_ = nullptr;
    bool inepoll_ = false;
    uint32_t event_;
    uint32_t revent_;
public:
    Channel(int fd_, Epoll *ep);
    ~Channel();

    void setET();
    void enableIN();
    void enableOUT();
    void setRevent(uint32_t revent);
    void setinepoll();
    bool inepoll();

    int fd();
    uint32_t event();
    uint32_t revent();
}
*/
Channel::Channel(int fd, Epoll *ep) : fd_(fd), ep_(ep), loop_(new Eventloop)
{
}

Channel::~Channel()
{
}
void Channel::enableIN()
{
    event_ |= EPOLLIN;
    ep_->updatechannel(this);
}
void Channel::enableOUT()
{
    event_ |= EPOLLOUT;
    ep_->updatechannel(this);
}

void Channel::setET()
{
    event_ |= EPOLLET;
    // ep_->updatechannel(this);
}

void Channel::setRevent(uint32_t revent)
{
    revent_ = revent;
}
void Channel::setinepoll(bool flag)
{
    inepoll_ = flag;
}

bool Channel::inepoll()
{
    return inepoll_;
}

int Channel::fd()
{
    return fd_;
}
uint32_t Channel::event()
{
    return event_;
}
uint32_t Channel::revent()
{
    return revent_;
}

void Channel::handleevent()
{
    if (revent() & EPOLLIN)
    {
        readcallback_();
    }
    else
    {
        std::cout << "hello" << std::endl;
    }
}

void Channel::newconnect(Socket &sock)
{

    // InetAddress ac_addr{"0.0.0.0", 8080};
    Socket *ac_sock = new Socket(sock.accept(*ac_addr));
    std::cout << "Successful connection from " << inet_ntoa(ac_addr->getAddr().sin_addr) << ":" << ntohs(ac_addr->getAddr().sin_port) << std::endl;
    Channel *ac_ch = new Channel(ac_sock->fd(), ep_);
    ac_ch->setcallback(std::bind(&Channel::onmessage, ac_ch));
    ac_ch->setET();
    ac_ch->enableIN();
}

void Channel::onmessage()
{
    char buff[1024];
    bzero(buff, sizeof(buff));
    ssize_t size = read(fd(), buff, sizeof(buff));
    if (size > 0)
    {
        loop_->thread_pool()->add_task([this, buff, size]()
                                       {
                                        std::cout << "[" << fd() << "]: " << buff << std::endl;
                                        // std::cout << "Thread " << std::this_thread::get_id() << " processing message: " << buff << std::endl;
                                        write(fd(), buff, size); });
    }
    else if (size < 0 and (errno == EAGAIN or errno == EWOULDBLOCK))
    {
    }
    else if (size == 0)
    {
        ep_->removechannel(this);
        close(fd());
        // std::cout << "client close" << std::endl;
    }
    else
    {
        exit(-1);
    }
}

void Channel::setcallback(std::function<void()> func)
{
    readcallback_ = func;
}
