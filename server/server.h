#ifndef SERVER_H
#define SERVER_H

//#include <vector>
#include <map>
#include <string>
#include <pthread.h>
#include <netinet/in.h>
#include <memory>
#include "network_socket/network_socket.h"

class ChatServer;
class ChatServerTest;


struct ClientData {
    int clientSocket;
    std::string clientUsername;
    ChatServer* server;
};

class ChatServer {
public:
    ChatServer(const std::string& ip, int port, std::unique_ptr<SocketInterface> ptr);
    ~ChatServer();
    void run();

private:
    static void* handleClient(void* arg);
    void broadcastMessage(const std::string& message, int senderSocket);
    int serverSocket;
    sockaddr_in serverAddr;
    //std::vector<int> clientSockets;
    pthread_mutex_t clientMutex;
    std::unique_ptr<SocketInterface> m_networkSocketPtr;
    int m_port;

    // User registration
    std::map<int,std::string> m_clientUsers;
};

#endif // SERVER_H