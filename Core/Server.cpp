#include "Server.h"
#include "Client.h"
#include <iostream>

Server::Server(const std::string& port) : listenSocket(INVALID_SOCKET), port(port), isRunning(false)
{
	
}

Server::~Server()
{
	stop();
}

bool Server::initializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "[ERROR] WSAStartup failed with error: " << result << std::endl;
        return false;
    }
    std::cout << "[INFO] WinSock initialized successfully" << std::endl;
    return true;
}

bool Server::start() {
    std::cout << "[INFO] Starting server..." << std::endl;

    if (!initializeWinsock()) return false;
    if (!createListenSocket()) return false;
    if (!startListening()) return false;

    isRunning = true;
    std::cout << "[INFO] Server started successfully!" << std::endl;
    return true;
}

void Server::stop() {
    if (!isRunning) return;

    std::cout << "[INFO] Shutting down server..." << std::endl;

    for (Client* client : clients) {
        delete client;
    }
    clients.clear();

    if (listenSocket != INVALID_SOCKET) {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }

 
    WSACleanup();
    isRunning = false;

    std::cout << "[INFO] Server stopped" << std::endl;
}

bool Server::createListenSocket() {
 
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;     

    struct addrinfo* addrResult = NULL;
    int result = getaddrinfo(NULL, port.c_str(), &hints, &addrResult);
    if (result != 0) {
        std::cerr << "[ERROR] getaddrinfo failed: " << result << std::endl;
        return false;
    }

    listenSocket = socket(addrResult->ai_family,
        addrResult->ai_socktype,
        addrResult->ai_protocol);

    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addrResult);
        return false;
    }

    result = bind(listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    freeaddrinfo(addrResult);

    if (result == SOCKET_ERROR) {
        std::cerr << "[ERROR] Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }

    std::cout << "[INFO] Socket created and bound to port " << port << std::endl;
    return true;
}

bool Server::startListening() {
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[ERROR] Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
        return false;
    }
    std::cout << "[INFO] Server listening on port " << port << std::endl;
    return true;
}


void Server::acceptClient() {
    if (!isRunning) {
        std::cerr << "[ERROR] Server is not running!" << std::endl;
        return;
    }

    std::cout << "[INFO] Waiting for client connection..." << std::endl;

    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "[ERROR] Accept failed: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "[INFO] Client connected!" << std::endl;

    Client* client = new Client(clientSocket);
    clients.push_back(client);

    client->handleCommunication(); 

    delete client;
    clients.pop_back();
}




