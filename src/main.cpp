#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include <iostream>

int main() {
    Server& server = Server::Singleton();
    if (server.initialize("password", 8080) != 0) {
        std::cerr << "Server initialization failed" << std::endl;
        return -1;
    }
    server.serverLoop();
    return 0;
}