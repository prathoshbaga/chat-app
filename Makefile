# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -I.

# Directories
SRC_DIR = .
BUILD_DIR = build
SERVER_DIR = server
CLIENT_DIR = client

#testing to remove later
NETWORK_SOCKET_DIR = network_socket

# Source Files
SERVER_SRC = $(SERVER_DIR)/main.cpp $(SERVER_DIR)/server.cpp
CLIENT_SRC = $(CLIENT_DIR)/main.cpp $(CLIENT_DIR)/client.cpp

#testing to remove later
NETWORK_SOCKET_HEADERS = $(NETWORK_SOCKET_DIR)/network_socket_interface.h $(NETWORK_SOCKET_DIR)/network_socket.h

# Binary Outputs
SERVER_BIN = $(BUILD_DIR)/server
CLIENT_BIN = $(BUILD_DIR)/client-cli

# Targets
.PHONY: all clean build

all: build $(SERVER_BIN) $(CLIENT_BIN)

build:
	mkdir -p $(BUILD_DIR)

# Build server binary
$(SERVER_BIN): $(SERVER_SRC) $(NETWORK_SOCKET_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(PWD)/$(NETWORK_SOCKET_DIR) -o $@ $(SERVER_SRC)

# Build client binary
$(CLIENT_BIN): $(CLIENT_SRC) $(NETWORK_SOCKET_HEADERS)
	$(CXX) $(CXXFLAGS) -I$(PWD)/$(NETWORK_SOCKET_DIR) -o $@ $(CLIENT_SRC)

# Cleanup the build folder
clean:
	rm -rf $(BUILD_DIR)
