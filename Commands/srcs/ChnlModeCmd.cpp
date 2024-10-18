#include "../includes/ChnlModeCmd.hpp"
#include <cstdlib>
#include <algorithm>

ChnlModeCmd::ChnlModeCmd()
{
}

ChnlModeCmd::~ChnlModeCmd()
{
}

void	ChnlModeCmd::execute(Client *client, IRCMessage const &message)
{
	std::string modes = message.getParams()[1];
	Channel *channel = Server::Singleton().getChannelByName(message.getParams()[0]);
	size_t paramIndex = 2;

	std::string modesToApply;
	std::vector<std::string> paramsToApply;
	if (!parseModes(modes, message.getParams(), paramIndex, client, channel, modesToApply, paramsToApply))
		return ;
	applyModes(channel, modesToApply, paramsToApply, client);
	std::string modeMessage = ":" + client->getNickName() + " MODE " + message.getParams()[0] + " " + modesToApply;
	for (size_t i = 0; i < paramsToApply.size(); ++i)
		modeMessage += " " + paramsToApply[i];
	modeMessage += "\r\n";
	channel->sendToAll(modeMessage);
}

bool	ChnlModeCmd::validate(IRCMessage const&msg)
{
	Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
	std::string message;
	if (msg.getParams().empty() || msg.getParams().size() < 1)
	{
		message = ":Server 461 MODE :Not enough parameters\r\n";
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	std::string channelName = msg.getParams()[0];
	//std::cout << "ChannelName: " << channelName << std::endl;
	if (Server::Singleton().getChannelByName(channelName) == 0)
	{
		message = ":Server 403 MODE :No such channel\r\n";
		//std::cout << message << std::endl;
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->getClientByNickName(client->getNickName()) == 0)
	{
		message = ":Server 442 " + channelName + " :You are not a member of the channel\r\n";
		//std::cout << message << std::endl;
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (Server::Singleton().getChannelByName(channelName)->isOperator(client) == false)
	{
		message = ":Server 482 " + channelName + " :You're not channel operator\r\n";
		//std::cout << message << std::endl;
		Server::Singleton().sendMsg(client, message);
		return false;
	}
	if (msg.getParams().size() == 1)
	{
		message = ":Server 324 " + client->getNickName() + " " + channelName + " " + MODES + "\r\n";
		Server::Singleton().sendMsg(client,message);
	}
	std::string modes = msg.getParams()[1];
	//std::cout << "Modes: " << modes << std::endl;
	execute(client,msg);
	return true;
}

bool	ChnlModeCmd::parseModes(const std::string &modes, const std::vector<std::string> &params, size_t &paramIndex, Client *client, Channel *channel, std::string &modesToApply, std::vector<std::string> &paramsToApply)
{
	bool	success = true;
	bool	adding = true;
	std::string msg;
	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i] == '+')
			adding = true;
		else if (modes[i] == '-')
			adding = false;
		else
		{
			std::cout << "modes: " << modes << std::endl;
			if (!isValidMode(modes[i]))
			{
				msg = ":Server 472 " + client->getNickName() + " MODE :is unknown mode char to me\r\n";
				Server::Singleton().sendMsg(client, msg);
				success = false;
				continue;
			}
			std::string param;
			if (modes[i] == 'o' || modes[i] == 'k' || (modes[i] == 'l' && adding))
			{
				if (paramIndex >= params.size())
				{
					msg = ":Server 461 " + client->getNickName() + " MODE :Not enough parameters for mode " + modes[i] +  "\r\n";
					Server::Singleton().sendMsg(client, msg);
					success = false;
					continue;
				}
				param = params[paramIndex++];
			}
			modesToApply += ((adding ? "+" : "-") + modes[i]);
			if (modes[i] == 'o' || modes[i] == 'k' || (modes[i] == 'l' && adding))
				paramsToApply.push_back(param);
		}
	}
	return success;
}

void	ChnlModeCmd::applyModes(Channel *channel, const std::string &modesToApply, const std::vector<std::string> &paramsToApply, Client *client)
{
	bool adding = true;
	size_t paramIndex = 0;
	for (size_t i = 0; i < modesToApply.size(); ++i)
	{
		if (modesToApply[i] == '+')
			adding = true;
		else if (modesToApply[i] == '-')
			adding = false;
		else
		{
			std::string param;
			if (modeRequiresParam(modesToApply[i], adding))
				param = paramsToApply[paramIndex++];
			switch (modesToApply[i])
			{
				case 'i':
					channel->setInviteOnly(adding);
					break;
				case 't':
					channel->setTopicLock(adding);
					break;
				case 'k':
					if (adding)
						channel->setKey(param);
					else
						channel->removeKey();
					break;
				case 'l':
					if (adding && std::atoi(param.c_str()) > 0)
						channel->setLimit(std::atoi(param.c_str()));
					else
						channel->removeLimit();
					break;
				case 'o':
					Client *toSet = Server::Singleton().getClientByNickName(param);
					if (toSet != 0 && toSet != client)
					{
						if (adding)
							channel->addOperator(toSet);
						else
							channel->removeOperator(toSet);
					}
			}
		}
	}
}

bool	ChnlModeCmd::isValidMode(char mode)
{
	std::string validModes = "itkol";
	return validModes.find(mode) != std::string::npos;
}

bool	ChnlModeCmd::modeRequiresParam(char mode, bool adding)
{
	if (mode == 'o' || mode == 'k' || (mode == 'l' && adding))
		return true;
	return false;
}

//void	ChnlModeCmd::handleModeI(bool adding, Channel *channel)
//{
//	if (adding)
//	{
//		channel->getModes()->inviteOnly = true;
//		Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
//		std::string message =":" + client->getNickName() + " MODE " + channel->getChannelName() + " +i\r\n";
//		Server::Singleton().sendMsg(client, message);
//	}
//	else
//		channel->getModes()->inviteOnly = false;
//}

//void	ChnlModeCmd::handleModeT(bool adding, Channel *channel, std::string &trailing)
//{
//	Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
//	if (adding)
//	{
//		channel->getModes()->topicLock = true;
//		std::string message = ":" + client->getNickName() + " MODE " + channel->getChannelName() + " +t :" + trailing + "\r\n";
//		std::cout << "NEW_TOPIC_SET: " << message << std::endl;
//		Server::Singleton().sendMsg(client, message);
//	}
//	else
//	{
//		channel->getModes()->topicLock = false;
//		std::string message = ":" + client->getNickName() + " MODE " + channel->getChannelName() + " -t\r\n";
//		Server::Singleton().sendMsg(client, message);
//	}
//}
