#include "network_socket/network_socket_interface.h"
#include <gmock/gmock.h>

class MockSocket : public SocketInterface {
public:
    MOCK_METHOD(int, createSocket, (), (override));
    MOCK_METHOD(int, bindSocket, (int socketFd, const std::string& ip, int port), (override));
    MOCK_METHOD(int, listenSocket, (int socketFd, int backlog), (override));
    MOCK_METHOD(int, acceptConnection, (int socketFd), (override));
    MOCK_METHOD(int, createConnection, (int socketFd, const std::string& serverIp, int serverPort), (override));
    MOCK_METHOD(ssize_t, sendData, (int socketFd, const char* buffer, size_t length), (override));
    MOCK_METHOD(ssize_t, receiveData, (int socketFd, char* buffer, size_t length), (override));
    MOCK_METHOD(void, closeSocket, (int socketFd), (override));
};