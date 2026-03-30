#pragma once

#include <socket/socket.h>


namespace Net{
    class ClientSocket:public Socket{
    public:
        ClientSocket() = delete;
        ClientSocket(std::string&& temp_ip, int&& temp_port);
        ~ClientSocket() = default;
    };
}