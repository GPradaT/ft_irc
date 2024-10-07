#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
Channel::Channel()
{
    this->_limit = -1;
    this->_key = 0;
}

Channel::~Channel()
{

}

Channel& Channel::operator+=(Client const& cli)
{
    Channel *chan = Server::Singleton().getChannelByName(this->_name);
    Server::Singleton().addClientToChannel((Client*)&cli, chan);
    return *this;
}

Channel& Channel::operator-=(Client const& cli)
{
    Channel *chan = Server::Singleton().getChannelByName(this->_name);
    Server::Singleton().removeClientFromChannel((Client*)&cli, chan);
    return *this;
}

std::vector<Client*> *Channel::getClientsFromChannel()
{
    return &this->_clients;
}

std::vector<Client*> *Channel::getOperators()
{
    return &this->_operators;
}

Client *Channel::getClientByNickName(std::string name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->getNickName() == name)
            return this->_clients[i];
    }
    return 0;
}

Client *Channel::getClientByRealName(std::string name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i]->getRealName() == name)
            return this->_clients[i];
    }
    return 0;
}

struct pollfd *Channel::getClientFd(Client* client)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i] == client)
            this->_clients[i]->getFd();
    }
    return 0;
}

std::string Channel::getChannelName()
{
    return this->_name;
}

void    Channel::setName(const std::string &name)
{
    this->_name = name;
}
