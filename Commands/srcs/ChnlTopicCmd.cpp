#include "../includes/ChnlTopicCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlTopicCmd::ChnlTopicCmd()
{
}

ChnlTopicCmd::~ChnlTopicCmd()
{
}

void	ChnlTopicCmd::execute(Client *client, const IRCMessage &message)
{
	std::string channelName = message.getParams()[0];
	std::string topic = message.getTrailing();
	//std::string msg = "TOPIC " + channelName + " " + topic + "\r\n";
	std::string msg = ":" + client->getNickName() + " TOPIC " + channelName + " " + topic + "\r\n";
	if (Server::Singleton().getChannelByName(channelName)->getModes()->topicLock == false)
	{
		if (Server::Singleton().getChannelByName(channelName)->isOperator(client) == false)
		{
			msg = "ERR_CHANOPRIVSNEEDED :You're not channel operator\r\n";
			Server::Singleton().sendMsg(client, msg);
			return ;
		}
		else
			Server::Singleton().getChannelByName(channelName)->getModes()->Topic = topic;
	}
	Server::Singleton().getChannelByName(channelName)->sendToAll(msg);
}

bool	ChnlTopicCmd::validate(const IRCMessage &msg)
{
	Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	std::string message;
	if (msg.getParams().size() < 1)
	{
		std::cout << "ERRPARAMS 1" << std::endl;
		message = ":" + client->getNickName() + "ERR_NEEDMOREPARAMS TOPIC :Not enough parameters\r\n";
		Server::Singleton().sendMsg(client,message);
		return false;
	}
	std::string channelName = msg.getParams()[0];
	std::cout << channelName << std::endl;
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		std::cout << "ERRNOCHAN 2" << std::endl;
		message = ":" + client->getNickName() + " ERR_NOSUCHCHANNEL " + client->getNickName() + " " + channelName + " :No such channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->getClientByNickName(client->getNickName()) == 0)
	{
		std::cout << "ERRNOTINCHAN 3" << std::endl;
		message = ":ERR_NOTONCHANNEL " + client->getNickName() + " " + channelName + " :You're not on that channel\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (msg.getTrailing().empty() && msg.getParams().size() == 1)
	{
		std::cout << "RPLYES 4" << std::endl;
		if (Server::Singleton().getChannelByName(channelName)->getModes()->Topic.empty())
		{
			message = ":Server RPL_NOTOPIC " + client->getNickName() + " " + channelName + " :No topic is set\r\n";
			Server::Singleton().sendMsg(client, message);
			return false;
		}
		else
		{
			message = ":" + client->getNickName() + "RPL_TOPIC :" + Server::Singleton().getChannelByName(channelName)->getModes()->Topic + "\r\n";
			Server::Singleton().sendMsg(client, message);
			return false;
		}
	}
	execute(client, msg);
	return true;
}
