#include "../includes/ChnlJoinCmd.hpp"

ChnlJoinCmd::ChnlJoinCmd()
{
	_clFd = Server::Singleton().getCurrentFd();
	_client = Server::Singleton().getClientByFd(_clFd);
}

ChnlJoinCmd::~ChnlJoinCmd()
{
}

void	ChnlJoinCmd::execute()
{
	//send message to all clients in the channel
	//send message to the client that joined the channel
}

bool	ChnlJoinCmd::validate(IRCMessage &msg)
{
	//validate the command
	//return false if the command is invalid
	if (_client->getNick().empty())
	{
		Server::Singleton().sendMsg(_client, "ERR_NOTREGISTERED :You have not registered\r\n");
		return false;
	}
	if (msg.getParams().size() < 1)
	{
		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS JOIN :Not enough parameters\r\n");
		return false;
	}
	if (Server::Singleton().getChannelByName(msg.getParams()[0]) == 0)
	{
		Server::Singleton().createChannel(msg.getParams()[0]);
	}
	else if (checkChannelPermissions())
	{
		Server::Singleton().addClientToChannel(_client, Server::Singleton().getChannelByName(msg.getParams()[0]));
	}
	else
	{
		Server::Singleton().sendMsg(_client, "ERR_RESTRICTED :Your connection is restricted!\r\n");
		return false;
	}
	return true;
}
