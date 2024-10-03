#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

int Server::initialize(const std::string &psswd, const unsigned short &port)
{
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(port);
    this->_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_passwd = psswd;
    bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress));
    listen(this->_serverSocket, 5);
    struct pollfd server;
    this->_fds.push_back(server);
    this->_fds[0].fd = this->_serverSocket;
    this->_fds[0].events = POLLIN;
    return 0;
}

void    Server::serverLoop()
{

}

Channel    *Server::getChannelByName(const std::string &name)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i].getChannelName() == name)
            return &this->_channels[i];
    }
    return (nullptr);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return this->_clients[i].getFd();
    }
    return (nullptr);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return this->_clients[i].getFd();
    }
    return (nullptr);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return &this->_clients[i];
    }
    return (nullptr);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return &this->_clients[i];
    }
    return (nullptr);
}

int    Server::getServerSocket()
{
    return this->_serverSocket;
}

int Server::sendMsg(Client* client, const std::string &msg)
{
    send(client->getFd()->fd, msg.c_str(), msg.length(), 0);
    return 0;
}

int Server::sendMsgAll(const std::string &msg)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        send(this->_clients[i].getFd()->fd, msg.c_str(), msg.length(), 0);
    }
    return 0;
}

void Server::createChannel(const std::string &name)
{
    Channel newChannel;
    newChannel.setName(name);
    this->_channels.push_back(newChannel);
}

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd *fd)
{
    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setFd(fd);
    newClient.setAdmin(false);
    this->_clients.push_back(newClient);
}

int Server::addClientToChannel(Client *client, Channel *channel)
{
    if (client != nullptr)
    {
        (*channel->getClientsFromChannel()).push_back(client);
        if (client->isAdmin())
        {
            (*channel->getAdmins()).push_back(client);
        }
        return 0;
    }
    return -1;
}

int Server::moveClientFromToChannel(Client *client, Channel *from, Channel *to)
{
    if (client != nullptr)
    {
        (*from->getClientsFromChannel()).erase(std::find(begin(*from->getClientsFromChannel()), end(*from->getClientsFromChannel()), client));
        (*to->getClientsFromChannel()).push_back(client);
        if (client->isAdmin())
        {
            (*from->getAdmins()).erase(std::find(begin(*from->getAdmins()), end(*from->getAdmins()), client));
            (*to->getAdmins()).push_back(client);
        }
        return 0;
    }
    return -1;
}

Channel *Server::getChannelByClient(Client *client)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        for (int j = 0; j < (*this->_channels[i].getClientsFromChannel()).size(); j++)
        {
            if ((*this->_channels[i].getClientsFromChannel())[j] == client)
                return &this->_channels[i];
        }
    }
    return nullptr;
}