#include <socket/client_socket.h>
#include <functional>

Net::ClientSocket::ClientSocket(std::string&& temp_ip, int&& temp_port){
    set_read_buf(10 * 1024);
    set_send_buf(10 * 1024);
    set_linger(true, 0);
    set_keepalive();
    set_reuseaddr();

    // bind(std::forward<std::string>(temp_ip), std::move(temp_port));
    connect(temp_ip, std::forward<int> (temp_port));
    
}