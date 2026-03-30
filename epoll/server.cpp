#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include <wait.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENT 1024
// void handler(int sig)
// {
//     wait(NULL);
// }
int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("SOCKET: ");
        exit(1);
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(8080);
    if (bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("BIND: ");
        close(sock_fd);
        exit(1);
    }

    if (listen(sock_fd, 10) < 0)
    {
        perror("LISTEN: ");
        close(sock_fd);
        exit(1);
    };

    int epollfd = epoll_create(1);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sock_fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd, &ev);

    struct epoll_event events[MAX_EVENT];
    struct sockaddr_in ac_addr;
    socklen_t add_size = sizeof(struct sockaddr_in);
    int ac_sock;
    while (1)
    {
        int isready = epoll_wait(epollfd, events, MAX_EVENT, -1);
        for (int i = 0; i < isready; i++)
        {
            char send_buff[128]{""};
            char read_buff[8]{""};
            if (events[i].data.fd == sock_fd)
            {
                ac_sock = accept(sock_fd, (struct sockaddr *)&ac_addr, &add_size);

                if (ac_sock < 0)
                {
                    perror("ACCEPT: ");
                    // close(sock_fd);
                    exit(1);
                }
                int flag = fcntl(sock_fd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(ac_sock, F_SETFL, flag);

                ev.events = EPOLLIN or EPOLLOUT;
                ev.data.fd = ac_sock;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, ac_sock, &ev);
                std::cout << "ACCEPT A CLIENT'S REQUIREMENT! IP:" << inet_ntoa(ac_addr.sin_addr) << "  PORT:" << ntohs(ac_addr.sin_port) << std::endl;
                continue;
            }
            // else
            // {
            // while (true)
            // {
            else if (events[i].data.fd != sock_fd && events[i].events == EPOLLIN)
            {
                // std::cout << " read" << std::endl;

                int read_size = read(events[i].data.fd, read_buff, sizeof(read_buff));
                if (read_size < 0)
                {
                    perror("READ");
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
                else if (read_size > 0)
                {
                    std::cout << __TIME__ << std::endl;
                    std::cout << "FROM IP:" << inet_ntoa(ac_addr.sin_addr) << "; READ: ";
                    while (true)
                    {
                    }
                    events[i].events = EPOLLOUT;
                    epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &events[i]);
                    bzero(read_buff, sizeof(send_buff));
                }
                else
                {
                    std::cout << __TIME__ << std::endl;
                    std::cout << "FROM IP:" << inet_ntoa(ac_addr.sin_addr) << " DISCONNECT! " << std::endl;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
            }
            else
            {

                std::cout << "ENTER: ";
                fgets(send_buff, 128, stdin);
                send_buff[strlen(send_buff) - 1] = '\0';
                int send_size = write(events[i].data.fd, send_buff, sizeof(send_buff));
                if (send_size < 0)
                {
                    perror("SEND: ");
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                }
                else if (send_size > 0)
                {
                    // std::cout << "SEND: " << send_buff << std::endl;
                    bzero(send_buff, sizeof(send_buff));
                    events[i].events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &events[i]);
                }
            }
            // }
            // }
        }

        // close(ac_sock);
    }

    close(sock_fd);
    return 0;
}
