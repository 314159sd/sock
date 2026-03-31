#include <sys/socket.h>
#include <cstring>
#include <sys/epoll.h>
#include <InetAddress.h>
#include <Socket.h>
#include <epoll.h>
#include <Channel.h>
#include <Eventloop.h>

#define MAX_EVENT 1024

int main()
{

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("SOCKET: ");
        exit(1);
    }
    Socket sock(sock_fd);
    sock.set_reuse_addr(true);
    sock.set_nodelay(true);
    sock.set_nonblock(true);

    InetAddress sock_addr{"0.0.0.0", 8080};
    sock.bind(sock_addr);
    sock.listen();

    Eventloop epoll;
    Channel *sock_ch = new Channel(sock_fd, epoll.ep());
    sock_ch->setcallback(std::bind(&Channel::newconnect,sock_ch, std::ref(sock)));
    sock_ch->enableIN();

    epoll.run();
    
    close(sock_fd);
    return 0;
}
