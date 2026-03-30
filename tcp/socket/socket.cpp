#include <socket/socket.h>


Net::Socket::Socket():sock(0),ip(""),port(0){
    // Log::Logger::instance()->open("./sock.log");
    // debug("name=%s age=%d", "jack", 18);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        std::cerr << "Failed to Listen Socket! Errono: " << errno << "  msg: " << strerror(errno)<< std::endl;
    }
}

Net::Socket::Socket(int& sock_temp):sock(sock_temp),ip(""),port(0){};

Net::Socket::~Socket(){
    if (!(sock == 0)){
        close(sock);
    }
}

bool Net::Socket::bind(std::string&& temp_ip, int&& temp_port){
    struct sockaddr_in socket_add;
    memset(&socket_add, 0, sizeof(socket_add));
    socket_add.sin_family = AF_INET;
    if(temp_ip.empty()){
        socket_add.sin_addr.s_addr = INADDR_ANY;
    }
    else{
        socket_add.sin_addr.s_addr = inet_addr(temp_ip.c_str());
    }
    socket_add.sin_port = htons(temp_port);
    
    if (::bind(sock, (struct sockaddr*)&socket_add, sizeof(socket_add)) < 0){
        std::cerr << "FAIL TO BIND! Errono: " << errno << "    msg: " << strerror(errno) <<std::endl;
        return false;
    }
    else {
        ip = temp_ip;
        port = temp_port;
        return true;
    }
}

bool Net::Socket::connect(const std::string& temp_ip, int&& temp_port){
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(temp_ip.c_str());
    // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(temp_port);
    std::cout << inet_ntoa(server_addr.sin_addr)<< std::endl;

    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))){
        std::cerr << "FAILED TO CONNECT!" << std::endl;
        // debug("error: %d ,%s", errno, strerror(errno));
        return false;
    }
    else return true;
}

bool Net::Socket::listen(int&& backlog){
    if (::listen(sock, backlog) < 0){
        std::cerr << "FAIL TO LISTEN! Errono: " << errno << "   msg: " << strerror(errno) <<std::endl;
        return false;
    }
        else return true;
}

int Net::Socket::accept (){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof (client_addr);
    return ::accept(sock, (struct sockaddr*)&client_addr, &len);
}

std::string Net::Socket::read(){
    char buffer[1024];
    int read_size = ::read(sock, buffer, sizeof(buffer));
    if(read_size == -1){
        std::cerr << "Failed to Read!" << std::endl;
    }
    std::cout <<  "BUFF: " << buffer << std::endl ;
    return buffer;
}

bool Net::Socket::write(std::string&& msg){
    if(::write(sock, msg.c_str(), msg.length()) == -1){
        std::cerr << "FAIL TO WRITE" <<std::endl;
        return false;
    }
    else return true;
}

bool Net::Socket::set_non_blocking(){
    int flags = fcntl(sock, F_GETFL, 0);

    flags |= O_NONBLOCK;
    fcntl(sock, F_SETFL, flags);
    return true;
}

bool Net::Socket::set_send_buf(int&& size){
    int buff_size = size;
    if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size))< 0){
        std::cerr << "SEND ERROR! Errno: " << errno << ", " << strerror(errno) << std::endl ;
    }
}

bool Net::Socket::set_read_buf(int&& size){
    int buff_size = size;
    if(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size))< 0){
        std::cerr << "READ ERROR! Errno: " << errno << ", " << strerror(errno) << std::endl ;
    }
}

bool Net::Socket::set_linger (bool&& activate, int&& seconds){
    struct linger l;
    std::memset(&l, 0, sizeof(l));
    l.l_onoff = activate ? 1:0;
    l.l_linger = seconds;
    if (setsockopt(sock, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0 ){
        std::cerr << "LINGER ERROR! Errno: " << errno << ", " << strerror(errno) << std::endl ;
        return false;
    }
    return true;
}

bool Net::Socket::set_keepalive(){
    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0){
        std::cerr << "KEEPALIVE ERROR! Errno: " << errno << ", " << strerror(errno) << std::endl ;
        return false;
    }
    return true;
}

bool Net::Socket::set_reuseaddr(){
    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0){
        std::cerr << "REUSEADD ERROR! Errno: " << errno << ", " << strerror(errno) << std::endl ;
        return false;
    }
    return true;
}


