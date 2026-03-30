#pragma once

#include <socket/socket.h>

struct connecter{
    int sock;
    struct sockaddr_in add;
};

namespace Net{
    class ServerSocket:public Socket{
    public:
        ServerSocket() = delete;
        ServerSocket(std::string&& temp_ip, int&& temp_port);
        ServerSocket(int& temp):Socket(temp){
        };
        ~ServerSocket() = default;
    };
}
