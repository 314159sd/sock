#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>

#define server_ip "127.0.0.1"
#define port 8080
int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        perror("SOCKET: ");
        exit(1);
    }
    // fcntl(sock_fd, F_SETFD, O_NONBLOCK);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(server_ip);
    sock_addr.sin_port = htons(port);
    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        perror("CONNECT: ");
        close(sock_fd);
        exit(1);
    }
    std::cout << "CONNECT SUCCESSFULLY! IP: " << server_ip << std::endl;
    char send_buff[128]{""};
    char read_buff[128]{""};

    while (true)
    {
        std::cout << "ENTER: ";
        fgets(send_buff, 128, stdin);
        send_buff[strlen(send_buff) - 1] = '\0';
        int send_size = write(sock_fd, send_buff, sizeof(send_buff));
        if (send_size < 0)
        {
            perror("SEND: ");
            close(sock_fd);
            exit(1);
        }
        else if (send_size > 0)
        {
            std::cout << "SEND: " << send_buff << std::endl;
            bzero(send_buff, sizeof(send_buff));
        }
        else
        {
            std::cout << "DISCONNECT!" << std::endl;
        }

        int read_size = read(sock_fd, read_buff, sizeof(read_buff));
        if (read_size < 0)
        {
            perror("READ: ");
            close(sock_fd);
            exit(1);
        }
        else if (read_size > 0)
        {
            std::cout << "READ: " << read_buff << std::endl;
            bzero(read_buff, sizeof(send_buff));
        }
    }

    close(sock_fd);
    return 0;
}
