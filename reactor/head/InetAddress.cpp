#include "InetAddress.h"

/*
class InetAddress
{
    struct sockaddr_in addr_;

public:
    InetAddress(const std::string &ip, uint16_t port);
    InetAddress(const sockaddr_in add) : addr_(add) {};
    ~InetAddress();

    const char *ip() const;
    uint16_t port() const;
    const sockaddr *add() const;
}
*/

InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);
}

InetAddress::InetAddress(const sockaddr_in add) : addr_(add) {}

InetAddress::~InetAddress()
{
}

const char *InetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
}

uint16_t InetAddress::port() const
{
    return ntohs(addr_.sin_port);
}

const sockaddr *InetAddress::add() const
{
    return (sockaddr *)&addr_;
}

sockaddr_in InetAddress::getAddr() const
{
    return addr_;
}