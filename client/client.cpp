/*
client code
*/
#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


ChatClient::ChatClient(const std::string& username, const std::string& serverIp, int serverPort, std::unique_ptr<SocketInterface>ptr)
    : m_username(username) ,
    m_networkSocketPtr(std::move(ptr)) {
    m_clientSocket = m_networkSocketPtr->createSocket();
    if (m_clientSocket < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    if (m_networkSocketPtr->createConnection(m_clientSocket,serverIp, serverPort) < 0) {
        throw std::runtime_error("Connection failed");
    }

    pthread_mutex_init(&m_messageMutex, nullptr);
    
}

ChatClient::~ChatClient() {
    m_networkSocketPtr->closeSocket(m_clientSocket);
    pthread_mutex_destroy(&m_messageMutex);
}

bool ChatClient::registerUser(){

    // User registration
    std::string username_registration = m_username;
    m_networkSocketPtr->sendData(m_clientSocket,username_registration.c_str(),username_registration.size());

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = m_networkSocketPtr->receiveData(m_clientSocket, buffer, sizeof(buffer));
    std::string message(buffer, bytesReceived);
    std::cout<<"Received message : "<< message <<std::endl;

    if (message == "Welcome to chat-app"){
        return true;
    }

    return false;
}

void ChatClient::sendMessages() {
    
    std::string message;
    while (true) {
        std::cout<<"> ";
        std::getline(std::cin, message);

        // if (message=="/exit"){
        //     std::cout<<"exiting!!"<<std::endl;
        //     exit(0);
        // } // Graceful exit from clients

        pthread_mutex_lock(&m_messageMutex);
        message = m_username + ": " + message; // Append username to message
        pthread_mutex_unlock(&m_messageMutex);
        m_networkSocketPtr->sendData(m_clientSocket, message.c_str(), message.size());
    }
}

void ChatClient::receiveMessages() {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = m_networkSocketPtr->receiveData(m_clientSocket, buffer, sizeof(buffer));
        if (bytesReceived <= 0) {
            std::cerr << "Disconnected from server" << std::endl;
            break;
        }

        std::cout << buffer << std::endl;
    }
}