#include "server.h"
#include <iostream>
#include <memory>

int main() {

    std::unique_ptr<SocketInterface> ptr_networkSocket = std::make_unique<NetworkSocket>();
    try {
        ChatServer server("127.0.0.1", 8080, std::move(ptr_networkSocket));
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    return 0;
}