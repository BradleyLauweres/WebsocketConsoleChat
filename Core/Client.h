#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <string>

#define BUFFER_SIZE 512

class Client {
private:
    SOCKET socket;
    bool isConnected;
    char recvBuffer[BUFFER_SIZE];

public:
 
    Client(SOCKET clientSocket);
    ~Client();

   
    void handleCommunication(); 
    int receiveData();
    bool sendData(const char* data, int length);


    bool getIsConnected() const { return isConnected; }
    SOCKET getSocket() const { return socket; }
};

#endif 