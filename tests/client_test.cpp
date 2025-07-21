#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "client.h"
#include "mock_network_socket.h"

using ::testing::_;
using ::testing::Return;

class ChatClientTest : public ::testing::Test {
protected:
    std::unique_ptr<MockSocket> mockSocket;

    void SetUp() override {
        mockSocket = std::make_unique<MockSocket>();
    }
};

/*
Test to simulate the successful client creation
*/
TEST_F(ChatClientTest, ClientConnectionSuccess) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(4));

    EXPECT_CALL(*mockSocket.get(), createConnection(_, _, _))
        .WillOnce(Return(0));

   ChatClient client("username", "127.0.0.1", 8080, std::move(mockSocket));
}

/*
Test to exception while client socket creation
*/
TEST_F(ChatClientTest, ClientSocketFailure) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(-1)); 

    try{
        ChatClient client("username", "127.0.0.1", 8080, std::move(mockSocket)); 
    } catch (const std::exception& e){
        EXPECT_STREQ("Socket creation failed", e.what());
    }
   
}

/*
Test to exception while connecting to server
*/
TEST_F(ChatClientTest, ClientConnectiontFailure) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(4));

    EXPECT_CALL(*mockSocket.get(), createConnection(_, _, _))
        .WillOnce(Return(-1)); 

    try{
        ChatClient client("username", "127.0.0.1", 8080, std::move(mockSocket)); 
    } catch (const std::exception& e){
        EXPECT_STREQ("Connection failed", e.what());
    }
   
}
