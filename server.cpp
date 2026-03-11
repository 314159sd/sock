#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include <wait.h>

void handler(int sig)
{
    wait(NULL);
}
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

    struct sockaddr_in ac_addr;
    socklen_t add_size = sizeof(struct sockaddr_in);
    int ac_sock;
    signal(SIGCHLD, handler);
    while (true)
    {
        ac_sock = accept(sock_fd, (struct sockaddr *)&ac_addr, &add_size);
        if (ac_sock < 0)
        {
            perror("ACCEPT: ");
            close(sock_fd);
            exit(1);
        }
        std::cout << "ACCEPT A CLIENT'S REQUIREMENT! IP:" << inet_ntoa(ac_addr.sin_addr) << "  PORT:" << ntohs(ac_addr.sin_port) << std::endl;

        char send_buff[128]{""};
        char read_buff[128]{""};
        pid_t pid = fork();
        if (pid == 0)
        {
            while (true)
            {
                int read_size = read(ac_sock, read_buff, sizeof(read_buff));
                if (read_size < 0)
                {
                    perror("READ: ");
                    close(ac_sock);
                    exit(1);
                }
                else if (read_size > 0)
                {

                    std::cout << __TIME__ << std::endl;
                    std::cout << "FROM IP:" << inet_ntoa(ac_addr.sin_addr) << "; READ: " << read_buff << std::endl;
                    bzero(read_buff, sizeof(send_buff));
                }

                std::cout << "ENTER: ";
                fgets(send_buff, 128, stdin);
                send_buff[strlen(send_buff) - 1] = '\0';
                int send_size = write(ac_sock, send_buff, sizeof(send_buff));
                if (send_size < 0)
                {
                    perror("SEND: ");
                    close(ac_sock);
                    exit(1);
                }
                else if (send_size > 0)
                {
                    // std::cout << "SEND: " << send_buff << std::endl;
                    bzero(send_buff, sizeof(send_buff));
                }
            }
        }
    }
    close(ac_sock);
    close(sock_fd);
    return 0;
}
