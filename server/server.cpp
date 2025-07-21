/*
server code
*/
#include "server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>

ChatServer::ChatServer(const std::string& ip, int port, std::unique_ptr<SocketInterface> ptr) : m_networkSocketPtr(std::move(ptr)), m_port(port){
    serverSocket = m_networkSocketPtr->createSocket();
    if (serverSocket < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (m_networkSocketPtr->bindSocket(serverSocket, ip, port) < 0) {
        throw std::runtime_error("Binding failed");
    }

    if (m_networkSocketPtr->listenSocket(serverSocket, 10) < 0) {
        throw std::runtime_error("Listening failed");
    }

    pthread_mutex_init(&clientMutex, nullptr);
}

ChatServer::~ChatServer() {
    m_networkSocketPtr->closeSocket(serverSocket);
    for (const auto& pair : m_clientUsers) {
        m_networkSocketPtr->closeSocket(pair.first);
    }
    /* for (const auto& clientSocket : clientSockets) {
        m_networkSocketPtr->closeSocket(clientSocket);
    }*/
    pthread_mutex_destroy(&clientMutex);
}

void ChatServer::run() {
    std::cout << "Server is listening on port " << m_port << std::endl;
    while (true) {
        int clientSocket = m_networkSocketPtr->acceptConnection(serverSocket);
        if (clientSocket < 0) {
            std::cerr << "Client accept failed" << std::endl;
            continue;
        }

        // User registration
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = m_networkSocketPtr->receiveData(clientSocket, buffer, sizeof(buffer));
        std::string message(buffer, bytesReceived);
        std::cout<<"Received message "<< message << " from "<< clientSocket<<std::endl;
        bool new_user = true;
        for (const auto& pair : m_clientUsers) {
            std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
            if (message == pair.second){
                std::cout<<message<<" Already exists."<<std::endl;
                new_user = false;
            }
        }

        if (new_user){
            std::string regMsg = "Welcome to chat-app";
            m_networkSocketPtr->sendData(clientSocket,regMsg.c_str(),regMsg.size());

            // Lock the mutex before modifying the clientSockets list
            pthread_mutex_lock(&clientMutex);
            //clientSockets.push_back(clientSocket);
            // User registration
            m_clientUsers[clientSocket] = message; // register new user
            pthread_mutex_unlock(&clientMutex);

            // Prepare client data to pass to the thread
            ClientData* data = new ClientData{clientSocket, message, this};

            // Does not make sense to use smart pointer as we would have to convert it
            // into raw to use it and then use delete anyway.
            //std::unique_ptr<ClientData> data = std::make_unique<ClientData>(clientSocket, this);

            // Create a new thread to handle the client
            pthread_t thread;
            pthread_create(&thread, nullptr, handleClient, (void*)data);
            pthread_detach(thread);  // Detach the thread so it cleans up automatically

            std::cout<<"new client joined : "<<message<<" - "<< clientSocket <<std::endl;
        } else { // close connection as user already exists
            std::string errorMsg = message + " username already exists!";
            m_networkSocketPtr->sendData(clientSocket,errorMsg.c_str(),errorMsg.size());
            m_networkSocketPtr->closeSocket(clientSocket);
        }
    }
}

void* ChatServer::handleClient(void* arg) {

    ClientData* data = (ClientData*)arg;
    ChatServer* server = data->server;
    int clientSocket = data->clientSocket;
    std::string clientUsername = data->clientUsername; 
    delete data;  // Free the memory as we no longer need it

    while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = server->m_networkSocketPtr->receiveData(clientSocket, buffer, sizeof(buffer));
        if (bytesReceived <= 0) {

            std::cout<<"Client "<<server->m_clientUsers[clientSocket]<<" disconnected!"<<std::endl;
            server->m_networkSocketPtr->closeSocket(clientSocket);

            pthread_mutex_lock(&server->clientMutex);
            auto it = server->m_clientUsers.find(clientSocket);
            if (it != server->m_clientUsers.end()){
                server->m_clientUsers.erase(it);
            }

            /* auto it = std::find(server->clientSockets.begin(), server->clientSockets.end(), clientSocket);
            if (it != server->clientSockets.end()) {
                server->clientSockets.erase(it);
            }
            */
           
            pthread_mutex_unlock(&server->clientMutex);

            break;
        }

        //std::cout<<"bytesReceived : "<<bytesReceived<<std::endl;
        if ((size_t)bytesReceived >= sizeof(buffer)) {
            std::cerr << "Warning: Message too long, truncating!" << std::endl;

            // Optionally, send a warning to the client
            const std::string warningMessage = "Error: Your message is too long and has been truncated.\n";
            server->m_networkSocketPtr->sendData(clientSocket, warningMessage.c_str(), warningMessage.size());

            // Truncate the message to avoid buffer overflow
            // Adjust to the buffer size minus one for null termination and
            // take 3 spaces to add client-id to differentiate the users
            bytesReceived = sizeof(buffer) - 1 ;
        }

        std::string message(buffer, bytesReceived);
        //message = std::to_string(clientSocket) + "-" + message; // Append client id to message

        // check if the message is empty. If yes, then no need to process it
        size_t colonPos = message.find(':');
        if (colonPos != std::string::npos && colonPos + 2 == message.size()) {
            continue;
        }
        
        // send the message once prepared
        server->broadcastMessage(message, clientSocket);
    }

    return nullptr;
}

void ChatServer::broadcastMessage(const std::string& message, int senderSocket) {
    pthread_mutex_lock(&clientMutex);
    std::cout<<"Sending to "<<m_clientUsers.size()<<" clients"<<std::endl;
    for (const auto& pair : m_clientUsers) {
        if (pair.first != senderSocket) {
            m_networkSocketPtr->sendData(pair.first, message.c_str(), message.size());
        }
    }

    // Simpler approach to have sockets in a vector
    /* Can add extra log to specify sender & receiver if needed on the server console
    for (const auto& clientSocket : clientSockets) {
        if (clientSocket != senderSocket) {
            m_networkSocketPtr->sendData(clientSocket, message.c_str(), message.size());
        }
    }*/
    pthread_mutex_unlock(&clientMutex);
}
