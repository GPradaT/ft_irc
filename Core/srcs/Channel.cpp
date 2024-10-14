#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

Channel::Channel()
{
    this->_limit = -1;
    this->_key = "";
	_mode.inviteOnly = false;
	_mode.topicChannel = false;
	//_mode.secretChannel = false;
	_mode.operOnly = false;
	_mode.chanCreator = "";
}

Channel::~Channel()
{

}

Channel &Channel::operator+=(Client *cli)
{
    Channel *chan = Server::Singleton().getChannelByName(this->_name);
    Server::Singleton().addClientToChannel(cli, chan);
    return *this;
}

Channel &Channel::operator-=(Client *cli)
{
    Channel *chan = Server::Singleton().getChannelByName(this->_name);
    Server::Singleton().removeClientFromChannel(cli, chan);
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

bool	Channel::isOperator(Client *client)
{
	for (int i = 0; i < _operators.size(); ++i)
	{
		std::cout << "operator: " << _operators[i]->getNickName() << std::endl;
		if (_operators[i] == client)
			return true;
	}
	return false;
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

void    Channel::sendToAll(const std::string &msg)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        Server::Singleton().sendMsg(this->_clients[i], msg);
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
	//if (name.length() > 50 || !isFirstChannelChar(name[0]))
	//	return false;
	this->_name = name.substr(1);
	//if (this->_name[0] == '#')
	//	this->_name = this->_name.substr(1);
	std::cout << "final channel name: [" << this->_name << "]\n"<< std::endl;
	this->_channelPrefix = name[0];
	return true;;
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

s_mode	*Channel::getModes()
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
