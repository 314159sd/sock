#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <sys/epoll.h>
#include <fcntl.h>
#include <thread>
#include <functional>

void send_data(int &sock)
{
    char str[256];
    while (true)
    {
        std::cin.getline(str, sizeof(str));
        std::string temp = str;
        if (!(strlen(str) == 0))
        {
            write(sock, temp.c_str(), temp.length());
            memset(str, '\0', sizeof(str));
        }
    }
}

void ac_data(int &sock)
{
    char buffer[1024];
    while (true)
    {
        int read_size = recv(sock, buffer, sizeof(buffer), NULL);
        if (read_size < 0)
        {
            perror("ERROR:");
            continue;
        }
        else if (read_size == 0)
        {
            perror("NONE:");
            continue;
        }
        else
        {
            std::cout << "BUFF: ";
            for (char i : buffer)
            {
                std::cout << i;
                if (i == '\0')
                {
                    std::cout << std::endl;
                    break;
                }
            }
        }
    }
}

void set_nonblocking(int &sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sock, F_SETFL, flags);
}

char str[256];
void epoll_init(int &sock)
{
    int epoll = epoll_create1(1);
    struct epoll_event event;
    event.data.fd = sock;
    event.events = EPOLLIN;
    epoll_ctl(epoll, EPOLL_CTL_ADD, sock, &event);

    while (true)
    {
        if (event.events & EPOLLOUT)
        {
            if (write(event.data.fd, str, sizeof(str)) < 0)
            {

                // perror("ERROR:");
                event.data.fd = event.data.fd;
                event.events = EPOLLIN;
                epoll_ctl(epoll, EPOLL_CTL_MOD, event.data.fd, &event);
                continue;
            }

            // memset(str, '\0', 1024);
            // }
            else
            {

                event.data.fd = event.data.fd;
                event.events = EPOLLIN;
                epoll_ctl(epoll, EPOLL_CTL_MOD, event.data.fd, &event);
                continue;
            }
        }
        else if (event.events & EPOLLIN)
        {
            static char buffer[1024];
            int read_size = read(event.data.fd, buffer, sizeof(buffer));
            if (read_size == -1)
            {
                std::cerr << "Failed to Read!" << std::endl;
                close(event.data.fd);
                break;
            }
            else if (read_size == 0)
            {
                event.data.fd = -1;
                event.events = EPOLLOUT;
                epoll_ctl(epoll, EPOLL_CTL_MOD, event.data.fd, &event);
                continue;
            }
            for (char i : buffer)
            {
                std::cout << i;
                if (i == '\0')
                {
                    std::cout << std::endl;
                    break;
                }
            }

            event.data.fd = event.data.fd;
            event.events = EPOLLOUT;
            epoll_ctl(epoll, EPOLL_CTL_MOD, event.data.fd, &event);
            memset(buffer, '\0', 1024);
            
        }
        // }

        // }

        else
        {
            perror("WRITE ERROR!");
        }
    }

    close(epoll);
}

void Net_Init()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // set_nonblocking(client_socket);
    if (client_socket == -1)
    {
        std::cerr << "FAILED TO CREATE SOCKET!" << std::endl;
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9999);
    std::cout << inet_ntoa(server_addr.sin_addr) << std::endl;

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cerr << "FAILED TO CONNECT!" << std::endl;
        std::cout << errno << "  " << strerror(errno) << std::endl;
        return;
    }

    // char buffer[1024];
    // char str[1024];
    // while (true)
    // {
    //     // std::cin.getline(str, 1024);
    //     // std::string temp = str;
    //     // if (!(strlen(str) == 0))
    //     // {
    //     //     write(client_socket, temp.c_str(), temp.length());
    //     //     memset(str, '\0', 1024);
    //     // }

    //     int read_size = read(client_socket, buffer, sizeof(buffer));
    //     if (read_size < 0)
    //     {
    //         continue;
    //     }
    //     std::cout << "BUFF: " << buffer << std::endl;
    // }
    // epoll_init(client_socket);
    send_data(client_socket);

    close(client_socket);
}

// #include <socket/client_socket.h>

int main()
{
    Net_Init();

    // class Net::Socket client;
    // if (client.connect("127.0.0.1", 1000)){
    //     while (true){
    //         client.write("Hello Server! ");
    //         std::string msg = client.read();
    //         if (!msg.empty()){
    //         std::cout << msg << std::endl;
    //         }
    //     }
    // }

    // class Net::ClientSocket client("127.0.0.1", 1000);
    // while (true){
    //     client.write("Hello Server! ");
    //     std::string msg = client.read();
    //     if (!msg.empty()){
    //     std::cout << msg << std::endl;
    //     }
    // }
}