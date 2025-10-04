#include "/coding/SideProjects/WebsocketConsoleChat/App/TcpClient.h"
#include <iostream>

int main() {
    std::cout << "=== TCP Echo Client ===" << std::endl;

    TcpClient client("127.0.0.1", "8080");

    if (!client.connect()) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return 1;
    }

    client.startChatLoop();

    client.disconnect();

    std::cout << "Client exiting..." << std::endl;
    return 0;
}