#include "Server.hpp"
int main()
{
    Server::Singleton().createChannel("test");
    Server::Singleton().createClient("PEPE", "PEPE", nullptr);
    std::cout << "Channel name is " << Server::Singleton().getChannelByName("test")->getChannelName() << std::endl;
    Channel *chan = Server::Singleton().getChannelByName("test");
    Client *client= Server::Singleton().getClientByRealName("PEPE");
    Server::Singleton().addClientToChannel(client, chan);
    std::cout << "The first client of " << Server::Singleton().getChannelByName("test")->getChannelName()
    << " is " << (*Server::Singleton().getChannelByName("test")->getClientsFromChannel())[0]->getNickName() << std::endl;
}