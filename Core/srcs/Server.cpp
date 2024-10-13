#include "../includes/Server.hpp"
#include <signal.h>


#include "../includes/IRCMessage.hpp"
#include "../../Commands/includes/AuthNickCmd.hpp"
#include "../../Commands/includes/AuthPassCmd.hpp"
#include "../../Commands/includes/MsgPrivmsgCmd.hpp"
#include "../../Commands/includes/ChnlJoinCmd.hpp"
#include "../../Commands/includes/ChnlWhoCmd.hpp"

Server::Server()
{
    this->_commands["NICK"] = new AuthNickCmd();
    this->_commands["PASS"] = new AuthPassCmd();
    this->_commands["PRIVMSG"] = new MsgPrivmsgCmd();
    this->_commands["JOIN"] = new ChnlJoinCmd();
    this->_commands["WHO"] = new ChnlWhoCmd();
}

Server::~Server()
{

}

int Server::initialize(const std::string &psswd, const unsigned short &port)
{
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int a;
    //int flags = fcntl(this->_serverSocket, F_GETFL, 0);
    //fcntl(this->_serverSocket, F_SETFL, flags | O_NONBLOCK);
    setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int));
    this->_serverAddress.sin_family = AF_INET;
    this->_serverAddress.sin_port = htons(port);
    this->_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_passwd = psswd;
    bind(this->_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress));
    listen(this->_serverSocket, 5);
    struct pollfd server;
    this->_fds.push_back(server);
    getAt(this->_fds, 0)->fd = this->_serverSocket;
    getAt(this->_fds, 0)->events = POLLIN;
    return 0;
}

void closeall(int signum)
{
    // Handle signal to close all connections
    std::cout << "Closing all connections..." << std::endl;
    close(Server::Singleton().getServerSocket());
    exit(signum);
}

void Server::serverLoop()
{
    signal(SIGINT, closeall);
    int polVal, tmp_fd, counter;
    while (1)
    {
        Server::Singleton()[0]->events = POLLIN;
        std::vector<struct pollfd> poll_array(this->_fds.begin(), this->_fds.end());
        polVal = poll(poll_array.data(), poll_array.size(), -1);
        counter = poll_array.size();
        std::list<struct pollfd>::iterator it = this->_fds.begin();
        for (size_t i = 0; i < poll_array.size(); ++i, ++it) {
            it->revents = poll_array[i].revents;
        }
        for (int i = 0; i < counter; i++)
        {
            Server::Singleton().setCurrentFd(getAt(this->_fds, i));
            struct pollfd *curr = Server::Singleton()[i];
            if (curr->revents == 0){
                continue;
            }
            if (curr->fd == Server::Singleton().getServerSocket())
            {
                // Function to accept connection and create client
                tmp_fd = accept(Server::Singleton().getServerSocket(), 0, 0);
                if (tmp_fd < 0)
                    continue;
                struct pollfd clientfd;
                clientfd.fd = tmp_fd;
                clientfd.events = POLLIN;
                std::cout << "cliente creado\n";
                std::cout << "el fd client es " << tmp_fd << std::endl;
                Server::Singleton().createClient("", "", clientfd);
                Server::Singleton()[0]->events = POLLOUT;
            }
            else
            {
                char buffer[1024] = {0};
                int recVal = 0;
                recVal = recv(curr->fd, buffer, sizeof(buffer), 0);
                std::cout << buffer << std::endl;
                std::string str = buffer;
                std::stringstream ss(str);
                std::string line;
                while (std::getline(ss, line, '\n'))
                {
                    IRCMessage message(line);
                    // if (message.getIsValid())
                    Server::Singleton() *= message;
                    if (curr->fd == -1)
                    {
                        Server::Singleton() -= curr;
                        break;
                    }
                }
                memset(buffer, 0, 1024);
            }
        }
    }
    // closing the socket.
    close(Server::Singleton().getServerSocket());
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
    client->getFd()->fd = -1;
    /*for (int i = 0; i < this->_channels.size(); i++)
        getAt(this->_channels, i) -= client;
    std::deque<Client>::iterator it = std::find(this->_clients.begin(), this->_clients.end(), *client);
    if (it != this->_clients.end())
    {
        std::cout << "el viejo size de clients es " << this->_clients.size() << std::endl;
        std::cout << "cliente encontrado y borrado" << std::endl;
        this->_clients.erase(std::remove(this->_clients.begin(), this->_clients.end(), *client), this->_clients.end());
        std::cout << "el nuevo size de clients es " << this->_clients.size() << std::endl;
    }*/
    return *this;
}

Server& Server::operator-=(struct pollfd *fd)
{
    /*std::deque<struct pollfd>::iterator it;
    for (it = this->_fds.begin(); it < this->_fds.end(); it++)
    {
        if ((*it).fd == -1)
        {
            std::cout << "el viejo size de fds es " << this->_fds.size() << std::endl;
            this->_fds.erase(it);
            std::cout << "el nuevo size de fds es " << this->_fds.size() << std::endl;
            break;
        }
    }*/
    return *this;
}

Server&			Server::operator*=(IRCMessage const& msg)
{
    if (this->_commands.find(msg.getCommand()) != this->_commands.end())
        this->_commands[msg.getCommand()]->validate(msg);
    return *this;
}

struct pollfd *Server::operator[](int idx)
{
    return getAt(this->_fds, idx);
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
        if (getAt(this->_channels, i)->getChannelName() == name)
            return getAt(this->_channels, i);
    }
    return (0);
}

struct pollfd    *Server::getClientFdByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (getAt(this->_clients, i)->getNickName() == name)
            return getAt(this->_clients, i)->getFd();
    }
    return (0);
}

struct pollfd    *Server::getClientFdByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (getAt(this->_clients, i)->getRealName() == name)
            return getAt(this->_clients, i)->getFd();
    }
    return (0);
}

Client*    Server::getClientByNickName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (getAt(this->_clients, i)->getNickName() == name)
            return getAt(this->_clients, i);
    }
    return (0);
}

Client*    Server::getClientByRealName(const std::string &name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (getAt(this->_clients, i)->getRealName() == name)
            return getAt(this->_clients, i);
    }
    return (0);
}

Client			*Server::getClientByFd(struct pollfd *fd)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (getAt(this->_clients, i)->getFd() == fd)
            return getAt(this->_clients, i);
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
        send(getAt(this->_clients, i)->getFd()->fd, msg.c_str(), msg.length(), 0);
    }
    return 0;
}

void Server::createChannel(const std::string &name)
{
	Channel newChannel;
	if (newChannel.setName(name))
	{
		//struct pollfd *fd = Server::Singleton().getCurrentFd();
		//Client *client = Server::Singleton().getClientByFd(fd);
		//newChannel.setChannelModes(client);
		this->_channels.push_back(newChannel);
	}
}

void Server::createClient(const std::string &nick, const std::string &real, struct pollfd fd)
{

    Client newClient;
    newClient.setNick(nick);
    newClient.setReal(real);
    newClient.setAdmin(false);
    this->_fds.push_back(fd);
    newClient.setFd(&this->_fds.back());
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    getpeername(fd.fd, (struct sockaddr*)&client_addr, &client_len);
    struct hostent* host_entry = gethostbyaddr(&client_addr.sin_addr, sizeof(client_addr.sin_addr), AF_INET);
    std::cout << "Client hostname: " << host_entry->h_name << std::endl;
    newClient.setHost(host_entry->h_name);
    struct sockaddr_in local_addr;
    socklen_t local_len = sizeof(local_addr);
    getsockname(fd.fd, (struct sockaddr*)&local_addr, &local_len);
    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local_addr.sin_addr, server_ip, sizeof(server_ip));
    std::string tmp = server_ip;
    newClient.setServer(tmp);
    this->_clients.push_back(newClient);
}

int Server::addClientToChannel(Client *client, Channel *channel)
{
    if (client != 0)
    {
        channel->getClientsFromChannel()->push_back(client);
        if (client->isOperator())
        {
            channel->getOperators()->push_back(client);
        }
        return 0;
    }
    return -1;
}

Channel *Server::getChannelByClient(Client *client)
{
    for (int i = 0; i < this->_channels.size(); i++)
    {
        for (int j = 0; j < getAt(this->_channels, i)->getClientsFromChannel()->size(); j++)
        {
            if (*getAt((*getAt(this->_channels, i)->getClientsFromChannel()), i) == client)
                return getAt(this->_channels, i);
        }
    }
    return 0;
}

int Server::removeClientFromChannel(Client *client, Channel *channel)
{
    /*if (client != 0)
    {
        std::deque<Client*> *clients = channel->getClientsFromChannel();
        std::deque<Client*>::iterator it = std::find((*clients).begin(), (*clients).end(), client);
        for (int i = 0; i < (*clients).size(); i++)
        {
            if (getAt(this->_clients, i) == client)
            {
                getAt(this->_clients, i) = 0;
                break;
            }
        }
        if (it != (*clients).end())
            (*clients).erase(std::remove((*clients).begin(), (*clients).end(), (Client*)0), (*clients).end());
        if (client->isOperator())
        {
            std::deque<Client*> *admins = channel->getOperators();
            std::deque<Client*>::iterator it2 = std::find((*admins).begin(), (*admins).end(), client);
            for (int i = 0; i < (*admins).size(); i++)
            {
                if (getAt(this->_admins, i) == client)
                {
                    getAt(this->_admins, i) = 0;
                    break;
                }
            }
            if (it2 != (*admins).end())
                (*admins).erase(std::remove((*admins).begin(), (*admins).end(), (Client*)0), (*admins).end());
        }
        return 0;
    }*/
    return -1;
}
