#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

Channel::Channel()
{
	this->_limit = -1;
	this->_key = "";
	_modes.inviteOnly = false;
	_modes.topicLock = false;
	_modes.Topic = "";
	_modes.chanCreator = "";
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

std::deque<Client*> *Channel::getClientsFromChannel()
{
    return &this->_clients;
}

std::deque<Client*> *Channel::getOperators()
{
    return &this->_operators;
}

bool	Channel::isOperator(Client *client) const
{
	for (int i = 0; i < _operators.size(); ++i)
	{
		//std::cout << "operator: " << _operators[i]->getNickName() << std::endl;
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
	this->_name = name;
	this->_channelPrefix = name[0];
	return true;;
}

char	Channel::getChannelPrefix() const
{
	return this->_channelPrefix;
}

void    Channel::sendMsgExcept(Client *c, const std::string &msg)
{
    for (int i = 0; i < this->_clients.size(); i++)
    {
        Client *cli = this->_clients[i];
        if (c != cli)
        {
            //std::cout << "name to send is " << this->_clients.size() << std::endl;
            Server::Singleton().sendMsg(cli, msg);
        }
    }
}

s_mode  *Channel::getModes()
{
	return &this->_modes;
}

void	Channel::setKey(const std::string &key)
{
	this->_key = key;
}

void	Channel::removeKey()
{
	this->_key = "";
}

const std::string	&Channel::getKey() const
{
	return this->_key;
}

void	Channel::setLimit(int limit)
{
	this->_limit = limit;
}

void	Channel::removeLimit()
{
	this->_limit = -1;
}

int	Channel::getLimit() const
{
	return this->_limit;
}

void	Channel::setInviteOnly(bool inviteOnly)
{
	this->_modes.inviteOnly = inviteOnly;
}

bool	Channel::isInviteOnly() const
{
	return this->_modes.inviteOnly;
}

void	Channel::setTopicLock(bool topicLock)
{
	this->_modes.topicLock = topicLock;
}

bool	Channel::isTopicLocked() const
{
	return this->_modes.topicLock;
}

const std::string	&Channel::gettopicLock() const
{
	return this->_modes.Topic;
}

void	Channel::addOperator(Client *client)
{
	//std::deque<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
	//if (it == _operators.end())
	this->_operators.push_back(client);
}

void	Channel::removeOperator(Client *client)
{
	for (int i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i] == client)
		{
			this->_operators.erase(this->_operators.begin() + i);
			return;
		}
	}
}

void	Channel::addInvite(std::string &nickname)
{
	this->_inviteList.push_back(nickname);
}

bool	Channel::isInvited(std::string &nickname) const
{
	for (int i = 0; i < this->_inviteList.size(); i++)
	{
		if (this->_inviteList[i] == nickname)
			return true;
	}
	return false;
}
