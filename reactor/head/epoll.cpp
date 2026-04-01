#include "epoll.h"

/*
class Epoll
{
    static const int max_event{1024};
    int epoll_fd_;
    struct epoll_event events_[max_event];

public:
    Epoll();
    ~Epoll();

    void addfd(int fd, uint32_t op);
    void updatechannel(Channel *ch);
    std::vector<epoll_event> loop(int timeout);
};

*/
Epoll::Epoll()
{
    epoll_fd_ = epoll_create(1);
}

Epoll::~Epoll()
{
    ::close(epoll_fd_);
}

// void Epoll::addfd(int fd, uint32_t op)
// {
//     struct epoll_event event;
//     event.events = op;
//     event.data.fd = fd;
//     ::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
// }

void Epoll::updatechannel(Channel *ch)
{
    struct epoll_event event;
    event.events = ch->event();
    event.data.ptr = ch;
    if (ch->inepoll())
    {
        epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, ch->fd(), &event);
    }
    else
    {
        epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, ch->fd(), &event);
        ch->setinepoll(true);
    }
}

void Epoll::removechannel(Channel *ch)
{
    if (ch->inepoll())
    {
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, ch->fd(), nullptr);
        ch->setinepoll(false);
    }
    else{
        std::cout << "channel not in epoll" << std::endl;
    }
}
std::vector<Channel *> Epoll::loop(int timeout)
{

    std::vector<Channel *> channels;
    bzero(events_, sizeof(events_));
    int num = ::epoll_wait(epoll_fd_, events_, max_event, timeout);
    if (num < 0)
    {
        std::cout << __FILE__ << ":" << __LINE__ << __FUNCTION__ << std::endl;
        std::perror("epoll_wait");
        exit(1);
    }
    for (int i = 0; i < num; i++)
    {
        Channel *ch = (Channel *)events_[i].data.ptr;
        ch->setRevent(events_[i].events);
        channels.emplace_back(ch);
    }
    return channels;
}
