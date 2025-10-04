#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 512

class TcpClient {
private:
    SOCKET connectSocket;
    std::string serverAddress;
    std::string serverPort;
    bool isConnected;
    char recvBuffer[BUFFER_SIZE];

    bool initializeWinsock();

public:
    TcpClient(const std::string& address = "127.0.0.1",
        const std::string& port = "8080");
    ~TcpClient();

    bool connect();
    void disconnect();

    bool sendMessage(const std::string& message);
    std::string receiveMessage();

    void startChatLoop();

    bool getIsConnected() const { return isConnected; }
};

#endif