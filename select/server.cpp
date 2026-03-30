#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        perror("CREATE_SOCKET:");
    }

    struct sockaddr_in l_addr;
    l_addr.sin_family = AF_INET;
    l_addr.sin_addr.s_addr = INADDR_ANY;
    l_addr.sin_port = htons(8080);

    if (bind(sock_fd, (sockaddr *)&l_addr, sizeof(l_addr)) < 0)
    {
        perror("BIND_ERROR:");
    }

    if (listen(sock_fd, 256) == -1)
    {
        perror("LISTEN_ERROR:");
    }

    fd_set redset;
    FD_ZERO(&redset);
    FD_SET(sock_fd, &redset);
    int max{sock_fd};
    char read_buff[128];
    char send_buff[128];

    //  pthread_t thread;
    while (true)
    {
        fd_set temp{redset};
        select(max + 1, &temp, NULL, NULL, NULL);
        //  pthread_create(&thread, NULL, );
        if (FD_ISSET(sock_fd, &temp))
        {
            int ac_fd = accept(sock_fd, NULL, NULL);
            if (ac_fd < 0)
            {
                perror("ACCEPT_ERROR");
            }
            std::cout << "accept client  : " << ac_fd << std::endl;
            fcntl(ac_fd, F_SETFD, O_NONBLOCK);
            max = max > ac_fd ? max : ac_fd;
            FD_SET(ac_fd, &redset);
        }
        for (int i = 0; i <= max; i++)
        {
            if (i != sock_fd && FD_ISSET(i, &temp))
            {
                while (true)
                {
                    int read_size = read(i, read_buff, sizeof(read_buff));
                    if (read_size < 0)
                    {
                        // perror("READ");
                        close(i);
                    }

                    else if (read_size == 0)
                    {
                        std::cout << __TIME__ << std::endl;
                        std::cout << "DISCONNECT! " << std::endl;
                        close(i);
                    }
                    else
                    {

                        std::cout << __TIME__ << std::endl;
                        std::cout << "READ: " << read_buff << std::endl;
                    }
                    write(i, read_buff, sizeof(read_buff));
                    bzero(read_buff, sizeof(send_buff));
                    // std::cout << "ENTER: ";
                    // fgets(send_buff, 128, stdin);
                    // send_buff[strlen(send_buff) - 1] = '\0';
                    // int send_size = write(i, send_buff, sizeof(send_buff));
                    // if (send_size < 0)
                    // {
                    //     perror("SEND: ");
                    //     close(i);
                    //     exit(1);
                    // }
                    // else if (send_size > 0)
                    // {
                    //     // std::cout << "SEND: " << send_buff << std::endl;
                    //     bzero(send_buff, sizeof(send_buff));
                    // }
                }
            }
        }
    }
    close(sock_fd);
}