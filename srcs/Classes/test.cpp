#include "Server.hpp"

int main()
{
    Server::Singleton().initialize("1234", 5555);
    //std::cout << "valor del singleton es " << Server::Singleton().getServerSocket() << std::endl;
}