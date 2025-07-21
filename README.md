# chat-app

The chat-app is a simple client-server chat application using C++, BSD socket & POSIX primitives.
The chat allows users to register to the server(**127.0.0.1:8080**) & send messages to other connected clients.
The application provides flexibility in terms of creating the server & clients independently.

PS - The current implementation limits only to create one server at 127.0.0.1:8080, however, the code can extended similar to the client.
Where we the server binary can accept the ip & the port and create the srever as desired.

## 1 Installation
### 1.1 Install bianries
```make```
This generates the server & client binaries under build/ as **server** & **client-cli** respectively

### 1.2 Install unit-tests
If you want to install & run tests
```make -f Makefile.tests run_tests```

If you want to install 
```make -f Makefile.tests```
This generates the server & client test binaries under build/ as **server_test** & **client_test** respectively

## 2 Usage
### 2.1 Create the server
```./build/server ```

### 2.2 Create clients
```./build/client-cli <user_name> 127.0.0.1 8080```
You can create multiple clients in multiple windows.
Incase if the user is successfully registered you'd see on the client console : "welcome to chat-app"
Incase if the username is already taken, then you'd see something like "username is already taken."

### 2.3 Send/Receive messages
From any of the client console type a message and you will notice that the sent message appears on console of other clients
```> Hello from user1```

Follwoing message will be seen on other clients
```> user1: Hello from user1```

## 3 Additional
The initial implementation uniquely identified the user based on the combination of the username & the socket id assigned to it.
Therefore if we have mutiple users with the same name eg. user_name, their messages are identified based on their socket-id.
"3-user_name" or "4-user_name" etc
The code is currently commented out. This is a much simpler logic.

Inorder for a much sophisticated unique client handling, in the latest version we use a std::map<int,std::string> in the server.
Here the client socket-id is mapped against the username. So when the client registers, it sends its username to the server.
The server checks against its list & allows for further connection only if the username is unique.

