#include <socket/server_socket.h>


int main(int, char**){

    class Net::ServerSocket server_temp("127.0.0.1", 1000);
    int temp = server_temp.accept();
    class Net::ServerSocket server(temp);

     while(true){
        std::string msg = server.read();
        server.write("Hello Cilent! ");
        if (!msg.empty()){
            std::cout << msg << std::endl;
            continue;
        }
    }
}
