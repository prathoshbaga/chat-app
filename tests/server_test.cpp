#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "server.h"
#include "mock_network_socket.h"

using ::testing::_;
using ::testing::Return;

class ChatServerTest : public ::testing::Test {
protected:
    std::unique_ptr<MockSocket> mockSocket;

    void SetUp() override {
        mockSocket = std::make_unique<MockSocket>();
    }
};


/*
Test to simulate the successful server creation
*/
TEST_F(ChatServerTest, ServerCreationSuccess) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(3));

    EXPECT_CALL(*mockSocket.get(), bindSocket(3, "127.0.0.1", 8080))
        .WillOnce(Return(0));

    EXPECT_CALL(*mockSocket.get(), listenSocket(3, _))
        .WillOnce(Return(0));

    ChatServer server("127.0.0.1", 8080, std::move(mockSocket));
}


/*
Test to simulate the exception while socket creation
*/
TEST_F(ChatServerTest, ServerSocketFailure) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(-1)); // simulate socket creation failure

    try {
        ChatServer server("127.0.0.1", 8080, std::move(mockSocket));
        FAIL() << "Expected exception was not thrown";
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Socket creation failed", e.what());
    }
}

/*
Test to simulate the exception while binding
*/
TEST_F(ChatServerTest, ServerBindingFailure) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(3));

    EXPECT_CALL(*mockSocket.get(), bindSocket(3, "127.0.0.1", 8080))
        .WillOnce(Return(-1)); // simulate binding failure

    try {
        ChatServer server("127.0.0.1", 8080, std::move(mockSocket));
        FAIL() << "Expected exception was not thrown";
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Binding failed", e.what());
    }
}

/*
Test to simulate the execpetion while listening
*/
TEST_F(ChatServerTest, ServerListeningFailure) {
    EXPECT_CALL(*mockSocket.get(), createSocket())
        .WillOnce(Return(3));

    EXPECT_CALL(*mockSocket.get(), bindSocket(3, "127.0.0.1", 8080))
        .WillOnce(Return(0));

    EXPECT_CALL(*mockSocket.get(), listenSocket(3, _))
        .WillOnce(Return(-1));

    try {
        ChatServer server("127.0.0.1", 8080, std::move(mockSocket));
        FAIL() << "Expected exception was not thrown";
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Listening failed", e.what());
    }
}

