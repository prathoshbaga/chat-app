#include "network_socket_interface.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

class NetworkSocket : public SocketInterface {
public:
    int createSocket() override {
        return socket(AF_INET, SOCK_STREAM, 0);
    }

    int bindSocket(int socketFd, const std::string& ip, int port) override {
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
        return bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    int listenSocket(int socketFd, int backlog) override {
        return listen(socketFd, backlog);
    }

    int acceptConnection(int socketFd) override {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        return accept(socketFd, (struct sockaddr*)&clientAddr, &clientLen);
    }

    int createConnection(int socketFd, const std::string& serverIp, int serverPort) override {
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
        return connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    ssize_t sendData(int socketFd, const char* buffer, size_t length) override {
        return send(socketFd, buffer, length, 0);
    }

    ssize_t receiveData(int socketFd, char* buffer, size_t length) override {
        return recv(socketFd, buffer, length - 1, 0);
    }

    void closeSocket(int socketFd) override {
        close(socketFd);
    }
};