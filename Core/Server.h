#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class Client;

class Server
{
private:
	SOCKET listenSocket;
	std::string port;
	bool isRunning;
	std::vector<Client*> clients;

	bool initializeWinsock();
	bool createListenSocket();
	bool startListening();

	public:
		Server(const std::string& port = "8080");
		~Server();


		bool start();
		void acceptClient();
		void stop();

	
		bool getIsRunning() const { return isRunning; }
};

#endif 
