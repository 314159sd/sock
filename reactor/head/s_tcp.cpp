#include "s_tcp.h"

TCP::TCP(bool reuse_addr, bool no_delay, bool no_block)
    : reuse_addr(reuse_addr), no_delay(no_delay), no_block(no_block)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("SOCKET: ");
        exit(1);
    }

    Socket sock(sock_fd);
    sock.set_reuse_addr(reuse_addr);
    sock.set_nodelay(no_delay);
    sock.set_nonblock(no_block);

    InetAddress sock_addr{"0.0.0.0", 8080};
    sock.bind(sock_addr);
    sock.listen();

    Eventloop epoll;
    Channel *sock_ch = new Channel(sock_fd, epoll.ep());
    sock_ch->setcallback(std::bind(&Channel::newconnect, sock_ch, std::ref(sock)));
    sock_ch->enableIN();

    epoll.run();
}

TCP::~TCP()
{
    close(sock_fd);
}

