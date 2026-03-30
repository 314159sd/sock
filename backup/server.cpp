#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>
#include <fcntl.h>
#include <thread>
// #include <socket/server_socket.h>

char str[256]{"HELLO"};

void set_nonblocking(int &sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sock, F_SETFL, flags);
}

void epoll_init(int &listen_sock)
{
    int epoll = epoll_create1(0);
    struct epoll_event event;
    event.data.fd = listen_sock;
    event.events = EPOLLIN;
    epoll_ctl(epoll, EPOLL_CTL_ADD, listen_sock, &event);

    struct epoll_event events[1024];
    while (true)
    {

        int num = epoll_wait(epoll, events, 1024, -1);
        if (num < 0)
        {
            perror("Epoll_wait Error: ");
            close(listen_sock);
            break;
        }
        else if (num == 0)
        {
            std::cout << "NONE!" << std::endl;
        }
        else
        {
            for (int i = 0; i < num; ++i)
            {

                // if (events[i].events & EPOLLIN)
                // {
                if (events[i].data.fd == listen_sock)
                {
                    struct sockaddr_in client_addr;
                    socklen_t len = sizeof(client_addr);
                    int client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &len);
                    if (client_sock == -1)
                    {
                        std::cerr << "Failed to Accept!" << std::endl;
                        continue;
                    }
                    printf("accpet a new client: %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
                    set_nonblocking(listen_sock);

                    // struct epoll_event event;
                    event.data.fd = client_sock;
                    event.events = EPOLLIN;
                    epoll_ctl(epoll, EPOLL_CTL_ADD, client_sock, &event);
                }
                else if (events[i].events & EPOLLIN)
                {
                    static char buffer[1024];
                    int read_size = read(events[i].data.fd, buffer, sizeof(buffer));
                    if (read_size == -1)
                    {
                        perror("READ ERROR:");
                        close(events[i].data.fd);
                        std::cout << "Here is no connection!" << std::endl;
                        break;
                    }
                    else if (read_size == 0 || (read_size < 0 && errno != EAGAIN))
                    {
                        close(events[i].data.fd);
                        epoll_ctl(epoll, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        printf("Client disconnected\n");
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

                    event.data.fd = events[i].data.fd;
                    event.events = EPOLLOUT;
                    epoll_ctl(epoll, EPOLL_CTL_MOD, events[i].data.fd, &event);
                    memset(buffer, '\0', 1024);
                    continue;
                }
                // }
                else if (events[i].events & EPOLLOUT)
                {
                    if (write(events[i].data.fd, str, sizeof(str)) < 0)
                    {

                        // perror("ERROR:");
                        event.data.fd = events[i].data.fd;
                        event.events = EPOLLIN;
                        epoll_ctl(epoll, EPOLL_CTL_MOD, events[i].data.fd, &event);
                        continue;
                    }

                    // memset(str, '\0', 1024);
                    // }
                    else
                    {

                        event.data.fd = events[i].data.fd;
                        event.events = EPOLLIN;
                        epoll_ctl(epoll, EPOLL_CTL_MOD, events[i].data.fd, &event);
                    }
                }
                // }

                else
                {
                    perror("WRITE ERROR!");
                }
            }
        }
    }
    delete[] events;
    close(epoll);
}

// #include <mutex>
// char str[256];
// std::mutex mut;
// void getdata()
// {
//     std::unique_lock(mut);
//     std::cin.getline(str, sizeof(str));
//     memset(str, '\0', sizeof(str) + 1);
// }

int main(int, char **)
{
    // 1、创建Socket
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblocking(listen_sock);
    if (listen_sock == -1)
    {
        std::cerr << "Failed to Listen Socket! Errono: " << errno << "  msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2、绑定Socket
    // 初始化
    struct sockaddr_in server_add;
    memset(&server_add, 0, sizeof(server_add));
    // bzero(&server_add, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = INADDR_ANY;
    server_add.sin_port = htons(9999);
    // 绑定
    if (bind(listen_sock, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        std::cerr << "FAIL TO BIND! Errono: " << errno << "    msg: " << strerror(errno) << std::endl;
        return 1;
    }

    // 3、监听
    if (listen(listen_sock, 1024) < 0)
    {
        std::cerr << "FAIL TO LISTEN! Errono: " << errno << "   msg: " << strerror(errno) << std::endl;
        return 1;
    }

    epoll_init(listen_sock);
    // fd_set redset;
    //     FD_ZERO(&redset);
    // FD_SET(listen_sock, &redset);
    // int maxfd = listen_sock;
    // while(1){
    //     fd_set temp_redset = redset;
    //     int ret = select(maxfd+1, &temp_redset, NULL, NULL, NULL);
    //     if(FD_ISSET(listen_sock, &temp_redset)){
    //         int cfd = accept(listen_sock, NULL, NULL);
    //         FD_SET(cfd, &redset);
    //         maxfd = cfd>maxfd ? cfd:maxfd;
    //     }
    //     for ( int i=0; i <= maxfd; ++i){
    //         if (i != listen_sock && FD_ISSET(i, &temp_redset)){
    //             char buffer[1024];
    //             int read_size = read(i, buffer, sizeof(buffer));
    //             if(read_size == -1){
    //                 std::cerr << "Failed to Read!" << std::endl;
    //                 exit(0);
    //             }
    //             else if (read_size == 0)
    //             {
    //                 /* code */
    //                 std::cout << "Disconnect!" << std::endl;
    //                 FD_CLR(i, &redset);
    //                 break;
    //             }
    //             std::cout <<  "BUFF: " << buffer << std::endl ;

    //             std::string msg = "OK!";
    //             if(write(i, msg.c_str(), msg.length()) == -1){
    //                 std::cerr << "FAIL TO WRITE" <<std::endl;
    //                 return 1;
    //             }

    //         }
    //     }

    // }

    // //std::cout << "ALL IS OK!" << std::endl;
    // struct sockaddr_in client_addr;
    // socklen_t len = sizeof (client_addr);
    // int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &len);
    // if(client_sock == -1){
    //     std::cerr << "Failed to Accept!" << std::endl;
    //     return 1;
    // }
    // std::cout << "client_socket: " << client_sock << std::endl;
    // //4、交互
    // char buffer[1024];
    // int read_size = read(client_sock, buffer, sizeof(buffer));
    // if(read_size == -1){
    //     std::cerr << "Failed to Read!" << std::endl;
    //     return 1;
    // }
    // std::cout <<  "BUFF: " << buffer << std::endl ;

    // std::string msg = "OK!";
    // if(write(client_sock, msg.c_str(), msg.length()) == -1){
    //     std::cerr << "FAIL TO WRITE" <<std::endl;
    //     return 1;
    // }

    // 5、关闭
    //  close(client_sock);
    close(listen_sock);

    // class Net::Socket init;
    // // std::string temp_ip {""};
    // init.bind("", 1000);
    // init.listen(1024);
    // int temp = init.accept();
    // class Net::Socket server(temp);
    // while(true){
    //     std::string msg = server.read();
    //     server.write("Hello Cilent! ");
    //     if (!msg.empty()){
    //         std::cout << msg << std::endl;
    //         continue;
    //     }
    // }
    // std::string temp_ip = "127.0.0.1";
    // class Net::ServerSocket server_temp("127.0.0.1", 1000);
    // int temp = server_temp.accept();
    // class Net::ServerSocket server(temp);

    //  while(true){
    //     std::string msg = server.read();
    //     server.write("Hello Cilent! ");
    //     if (!msg.empty()){
    //         std::cout << msg << std::endl;
    //         continue;
    //     }
    // }
}
