#include "../includes/Server.hpp"

#include "../../Commands/includes/AuthNickCmd.hpp"

Server::Server()
{
    this->_commands["NICK"] = new AuthNickCmd();
}

Server::~Server()
{

}

int Server::initialize(const std::string &psswd, const unsigned short &port)
{
    std::cout << this->_fds.size();
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

Server& Server::operator+=(std::string const& chanName)
{
    if (getChannelByName(chanName) == 0)
        createChannel(chanName);
    return *this;
}

Server& Server::operator-=(Client *client)
{
    //delete client
    return *this;
}

Server&			Server::operator*=(IRCMessage const& msg)
{
    if (this->_commands.find(msg.getParams()[0]) != this->_commands.end())
        this->_commands[msg.getParams()[0]]->validate(msg);
    return *this;
}

struct pollfd *Server::operator[](int idx)
{
    return &this->_fds[idx];
}

std::string Server::getPasswd()
{
    return this->_passwd;    
}

struct pollfd   *Server::getCurrentFd()
{
    return this->_currentFd;    
}

void			Server::setCurrentFd(struct pollfd *current)
{
    this->_currentFd = current;
}

Channel    *Server::getChannelByName(const std::string &name)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        if (this->_channels[i].getChannelName() == name)
            return &this->_channels[i];
    }
    return (0);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return this->_clients[i].getFd();
    }
    return (0);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return this->_clients[i].getFd();
    }
    return (0);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getNickName() == name)
            return &this->_clients[i];
    }
    return (0);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getRealName() == name)
            return &this->_clients[i];
    }
    return (0);
}

Client			*Server::getClientByFd(struct pollfd *fd)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (this->_clients[i].getFd() == fd)
            return &this->_clients[i];
    }
    return (0);
}

int    Server::getServerSocket()
{
    return this->_serverSocket;
}

int				Server::getFdSize()
{
    return this->_fds.size();
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

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd fd)
{
    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setAdmin(false);
    this->_fds.push_back(fd);
    newClient.setFd(&this->_fds[this->_fds.size() - 1]);
    this->_clients.push_back(newClient);
}

int Server::addClientToChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        (*channel->getClientsFromChannel()).push_back(client);
        if (client->isOperator())
        {
            (*channel->getOperators()).push_back(client);
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
    return 0;
}

int Server::removeClientFromChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        std::vector<Client*> *clients = channel->getClientsFromChannel();
        std::vector<Client*>::iterator it = std::find((*clients).begin(), (*clients).end(), client);
        for (int i = 0; i < (*clients).size(); i++)
        {
            if ((*clients)[i] == client)
            {
                (*clients)[i] = 0;
                break;
            }
        }
        if (it != (*clients).end())
            (*clients).erase(std::remove((*clients).begin(), (*clients).end(), (Client*)0), (*clients).end());
        if (client->isOperator())
        {
            std::vector<Client*> *admins = channel->getOperators();
            std::vector<Client*>::iterator it2 = std::find((*admins).begin(), (*admins).end(), client);
            for (int i = 0; i < (*admins).size(); i++)
            {
                if ((*admins)[i] == client)
                {
                    (*admins)[i] = 0;
                    break;
                }
            }
            if (it2 != (*admins).end())
                (*admins).erase(std::remove((*admins).begin(), (*admins).end(), (Client*)0), (*admins).end());
        }
        return 0;
    }
    return -1;
}
