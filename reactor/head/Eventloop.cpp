#include "Eventloop.h"

Eventloop::Eventloop() : ep_(new Epoll)
{
}

Eventloop::~Eventloop()
{
    delete ep_;
}

void Eventloop::run()
{
    while (true)
    {
        auto channels = ep_->loop(-1);
        // std::cout << channels.size() <<std::endl;
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