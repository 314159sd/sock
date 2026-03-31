#include "Socket.h"

/*
class Socket
{
    const int fd_;

public:
    Socket(int fd);
    ~Socket();

    int fd() const;
    void bind(const InetAddress &addr);
    void listen(int num = 128);
    void accept();

    void set_reuse_addr(bool flag);
    void set_nodelay(bool flag);
    void set_nonblock(bool flag);

};
*/

Socket::Socket(int fd) : fd_(fd)
{
    set_reuse_addr(true);
    set_nodelay(true);
    // std::cout << "Create New Socket" << std::endl;
}

Socket::~Socket()
{
    close(fd_);
}

int Socket::fd() const
{
    return fd_;
}
void Socket::bind(const InetAddress &addr)
{
    if (::bind(fd_, addr.add(), sizeof(addr)) < 0)
    {
        std::cout << __FILE__ << ":" << __LINE__ << __FUNCTION__ << std::endl;
        std::perror("bind");
        exit(1);
    }
}

void Socket::listen(int num)
{
    if (::listen(fd_, num) < 0)
    {
        std::cout << __FILE__ << ":" << __LINE__ << __FUNCTION__ << std::endl;
        exit(1);
    }
}

int Socket::accept(InetAddress &addr)
{
    socklen_t size = sizeof(sockaddr);
    int fd = ::accept(fd_, (sockaddr *)addr.add(), &size);

    set_nonblock(true);
    if (fd < 0)
    {
        std::perror("accept");
        std::cout << __FILE__ << ":" << __LINE__ << __FUNCTION__ << std::endl;
        exit(1);
    }
    return fd;
}

void Socket::set_nodelay(bool flag)
{
    int opt = flag ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, sizeof(opt));
}

void Socket::set_nonblock(bool flag)
{
    if (flag)
    {
        int opt = fcntl(fd_, F_GETFL, 0) | O_NONBLOCK;
        fcntl(fd_, F_SETFL, opt);
    }
}

void Socket::set_reuse_addr(bool flag)
{
    int opt = flag ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}
