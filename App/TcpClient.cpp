#include "TcpClient.h"
#include <iostream>

TcpClient::TcpClient(const std::string& address, const std::string& port)
    : connectSocket(INVALID_SOCKET), serverAddress(address),
    serverPort(port), isConnected(false) {
}

TcpClient::~TcpClient() {
    disconnect();
}

bool TcpClient::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "[ERROR] WSAStartup failed: " << result << std::endl;
        return false;
    }
    return true;
}

bool TcpClient::connect() {
    std::cout << "[INFO] Connecting to server..." << std::endl;

    if (!initializeWinsock()) return false;

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* addrResult = NULL;
    int result = getaddrinfo(serverAddress.c_str(), serverPort.c_str(),
        &hints, &addrResult);
    if (result != 0) {
        std::cerr << "[ERROR] getaddrinfo failed: " << result << std::endl;
        WSACleanup();
        return false;
    }

    connectSocket = socket(addrResult->ai_family,
        addrResult->ai_socktype,
        addrResult->ai_protocol);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return false;
    }

    result = ::connect(connectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    freeaddrinfo(addrResult);

    if (result == SOCKET_ERROR) {
        std::cerr << "[ERROR] Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    isConnected = true;
    std::cout << "[INFO] Connected to " << serverAddress << ":" << serverPort << std::endl;
    return true;
}

void TcpClient::disconnect() {
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }
    WSACleanup();
    isConnected = false;
    std::cout << "[INFO] Disconnected from server" << std::endl;
}

bool TcpClient::sendMessage(const std::string& message) {
    if (!isConnected) {
        std::cerr << "[ERROR] Not connected to server!" << std::endl;
        return false;
    }

    int bytesSent = send(connectSocket, message.c_str(), message.length(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "[ERROR] Send failed: " << WSAGetLastError() << std::endl;
        isConnected = false;
        return false;
    }

    std::cout << "[SENT] " << message << std::endl;
    return true;
}

std::string TcpClient::receiveMessage() {
    if (!isConnected) {
        std::cerr << "[ERROR] Not connected to server!" << std::endl;
        return "";
    }

    int bytesReceived = recv(connectSocket, recvBuffer, BUFFER_SIZE - 1, 0);

    if (bytesReceived > 0) {
        recvBuffer[bytesReceived] = '\0';
        return std::string(recvBuffer);
    }
    else if (bytesReceived == 0) {
        std::cout << "[INFO] Server closed connection" << std::endl;
        isConnected = false;
    }
    else {
        std::cerr << "[ERROR] Recv failed: " << WSAGetLastError() << std::endl;
        isConnected = false;
    }

    return "";
}

void TcpClient::startChatLoop() {
    std::string message;

    while (isConnected) {
        std::cout << "\nEnter message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") {
            break;
        }

        if (!sendMessage(message)) {
            break;
        }

        std::string echo = receiveMessage();
        if (!echo.empty()) {
            std::cout << "[ECHO] " << echo << std::endl;
        }
        else {
            break;
        }
    }
}