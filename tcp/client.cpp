#include <socket/client_socket.h>

int main(){
    Log::Logger::instance()->open("client.log");
    class Net::ClientSocket client("127.0.0.1", 1000);
    while (true){
        client.write("Hello Server! ");
        std::string msg = client.read();
        if (!msg.empty()){
        std::cout << msg << std::endl;
        }
    }
    return 0;
}