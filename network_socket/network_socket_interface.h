#ifndef NETWORK_SOCKET_INTERFACE_H
#define NETWORK_SOCKET_INTERFACE_H

#include <string>

class SocketInterface {
public:
    virtual ~SocketInterface() {}
    virtual int createSocket() = 0;
    virtual int bindSocket(int socketFd, const std::string& ip, int port) = 0;
    virtual int listenSocket(int socketFd, int backlog) = 0;
    virtual int acceptConnection(int socketFd) = 0;
    virtual int createConnection(int socketFd, const std::string& ip, int port) = 0;
    virtual ssize_t sendData(int socketFd, const char* buffer, size_t length) = 0;
    virtual ssize_t receiveData(int socketFd, char* buffer, size_t length) = 0;
    virtual void closeSocket(int socketFd) = 0;
};

#endif // NETWORK_SOCKET_INTERFACE_H