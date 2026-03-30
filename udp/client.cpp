#include <arpa/inet.h>
#include <iostream>
// #include <sys/fcntl.h>
// #include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <string.h>

#define ip "127.0.0.1"
#define port 8080
// #define char_size
int main(int argc, char const *argv[])
{
    // if (argc< 3){
    //     std::cerr << "ERROR!" << std::endl;
    // }
    int sock_fd{socket(AF_INET, SOCK_DGRAM, 0)};
    if (sock_fd < 0)
    {
        perror("SOCKET ERROR: ");
        exit(1);
    }
    // std::cout << sock_fd << std::endl;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    char buff[256]{""};
    char buf[256];
    while (true)
    {
        std::cout <<"ENTER: ";
        fgets(buff, 256, stdin);
        buff[sizeof(buff) - 1] = '\0';
        if (sendto(sock_fd, &buff, sizeof(buff), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("SEND ERROR: ");
            exit(1);
        }

        struct sockaddr_in client_addr;
        socklen_t size = sizeof(client_addr);
        if (recvfrom(sock_fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &size) < 0)
        {
            perror("READ ERROR: ");
            exit(1);
        }
        std::cout << __TIME__ << "; ";
        // std::cout << "IP: " << inet_ntoa(client_addr.sin_addr) << "  PORT:  " << ntohs(client_addr.sin_port) << std::endl;
        std::cout << "BUFF: " << buf << std::endl;
        bzero(buf, sizeof(buf));
    }
    close(sock_fd);
    return 0;
}
