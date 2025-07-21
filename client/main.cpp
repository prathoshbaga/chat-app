#include "client.h"
#include <iostream>
#include <thread>
#include "network_socket/network_socket.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: client-cli <username> <server-ip> <server-port>" << std::endl;
        return 1;
    }

    std::unique_ptr<SocketInterface> ptr_networkSocket = std::make_unique<NetworkSocket>();
    std::string username = argv[1];
    std::string serverIp = argv[2];
    int serverPort = std::stoi(argv[3]);

    try {
        ChatClient client(username, serverIp, serverPort, std::move(ptr_networkSocket));
        if (client.registerUser()){
            std::thread receiveThread(&ChatClient::receiveMessages, &client);
            client.sendMessages();
            receiveThread.join();
        } else {
            std::cerr<<"Registration failed"<<std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
    return 0;
}