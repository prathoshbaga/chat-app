#ifndef CLIENT_H
#define CLIENT_H

#include <string>
//#include <pthread.h>
#include <memory>
#include <set>
#include <netinet/in.h>
#include "network_socket/network_socket_interface.h"

class ChatClient {
public:
    ChatClient(const std::string& username, const std::string& serverIp, int serverPort, std::unique_ptr<SocketInterface>ptr);
    ~ChatClient();
    void sendMessages();
    void receiveMessages();
    bool registerUser();

private:
    int m_clientSocket;
    //sockaddr_in serverAddr;
    std::string m_username;
    pthread_mutex_t m_messageMutex;

    std::unique_ptr<SocketInterface> m_networkSocketPtr;
};

#endif // CLIENT_H