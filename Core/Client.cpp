#include "Client.h"
#include <iostream>

Client::Client(SOCKET clientSocket)
    : socket(clientSocket), isConnected(true) {
    std::cout << "[CLIENT] New client created (Socket: " << socket << ")" << std::endl;
}

Client::~Client() {
    if (socket != INVALID_SOCKET) {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }
    isConnected = false;
}

int Client::receiveData() {
    int bytesReceived = recv(socket, recvBuffer, BUFFER_SIZE, 0);

    if (bytesReceived > 0) {
        std::cout << "[CLIENT] Received " << bytesReceived << " bytes" << std::endl;
    }
    else if (bytesReceived == 0) {
        std::cout << "[CLIENT] Connection closed gracefully" << std::endl;
        isConnected = false;
    }
    else {
        std::cerr << "[CLIENT] Recv error: " << WSAGetLastError() << std::endl;
        isConnected = false;
    }

    return bytesReceived;
}

void Client::handleCommunication() {

    while (isConnected) {
        int bytesReceived = receiveData();

        if (bytesReceived > 0) {

            if (!sendData(recvBuffer, bytesReceived)) {
                break;
            }
        }
        else {
            break;
        }
    }

}

bool Client::sendData(const char* data, int length) {
    int bytesSent = send(socket, data, length, 0);

    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "[CLIENT] Send error: " << WSAGetLastError() << std::endl;
        isConnected = false;
        return false;
    }

    std::cout << "[CLIENT] Sent " << bytesSent << " bytes" << std::endl;
    return true;
}

