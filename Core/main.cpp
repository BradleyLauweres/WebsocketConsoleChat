#include "Server.h"
#include <iostream>

int main() {
    std::cout << "=== TCP Echo Server ===" << std::endl;

    Server server("8080");

    if (!server.start()) {
        std::cerr << "Failed to start server!" << std::endl;
        return 1;
    }
    server.acceptClient();

    server.stop();

    std::cout << "Server exiting..." << std::endl;
    return 0;
}