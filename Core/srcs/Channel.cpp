#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

Channel::Channel()
{
    this->_limit = -1;
    this->_key = "";
	_mode.inviteOnly = false;
	_mode.privateChannel = false;
	_mode.secretChannel = false;
	_mode.operOnly = false;
	_mode.chanCreator = "";
}

Channel::~Channel()
{

}

Channel &Channel::operator+=(Client *cli)
{
    Server::Singleton().addClientToChannel(cli, this);
    return *this;
}

Channel &Channel::operator-=(Client *cli)
{
    Server::Singleton().removeClientFromChannel(cli, this);
    return *this;
}

std::list<Client*> *Channel::getClientsFromChannel()
{
    return &this->_clients;
}

std::list<Client*> *Channel::getOperators()
{
    return &this->_operators;
}

Client *Channel::getClientByNickName(std::string name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if ((*getAt(this->_clients, i))->getNickName() == name)
            return *getAt(this->_clients, i);
    }
    return 0;
}

Client *Channel::getClientByRealName(std::string name)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if ((*getAt(this->_clients, i))->getRealName() == name)
            return *getAt(this->_clients, i);
    }
    return 0;
}

struct pollfd *Channel::getClientFd(Client* client)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        if (*getAt(this->_clients, i) == client)
            (*getAt(this->_clients, i))->getFd();
    }
    return 0;
}

std::string Channel::getChannelName()
{
    return this->_name;
}

void    Channel::sendToAll(const std::string &msg)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        Server::Singleton().sendMsg(*getAt(this->_clients, i), msg);
    }
}

bool	Channel::isFirstChannelChar(const char c) const
{
	if (c == '#' || c == '!')
		return true;
	return false;
}

bool	Channel::setName(const std::string &name)
{
	/*if (name.empty())
		return false;
	if (name.length() > 50 || !isFirstChannelChar(name[0]))
		return false;*/
	this->_name = name;
	this->_channelPrefix = name.c_str()[0];
	return true;
}

char	Channel::getChannelPrefix() const
{
	return this->_channelPrefix;
}

const std::string	&Channel::getKey() const
{
	return this->_key;
}

int	Channel::getLimit() const
{
	return this->_limit;
}

s_mode  *Channel::getModes()
{
    return &this->_mode;
}

void	Channel::setChannelModes(Client *client)
{
	//set the modes of the channel
	//set the creator of the channel
	//set the key of the channel
	//set the limit of the channel
	//set the modes of the channel
}
