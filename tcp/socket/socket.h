#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <log/Logger.h>
#include <sys/epoll.h>

using namespace Log;
 namespace Net{
    
    class Socket{
        
    private:
        int sock;
        std::string ip;
        int port;
        
    public:
        Socket();
        Socket(int& sock_temp);
        ~Socket();
        bool bind(std::string&& temp_ip, int&& temp_port);
        bool connect(const std::string& temp_ip, int&& temp_port);
        bool listen(int&& backlog );
        int accept();
        std::string read();
        bool write(std::string&& msg);

        bool set_non_blocking();
        bool set_send_buf(int&& size);
        bool set_read_buf(int&& size);
        bool set_linger (bool&& ativate, int&& seconds);
        bool set_keepalive();
        bool set_reuseaddr();
    };
 }

