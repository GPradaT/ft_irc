#include "../includes/ChnlJoinCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlJoinCmd::ChnlJoinCmd()
{

}

ChnlJoinCmd::~ChnlJoinCmd()
{
}

void	ChnlJoinCmd::execute(Client *client, IRCMessage const&message)
{
	//send message to all clients in the channel
	//send message to the client that joined the channel
	Channel *chan = Server::Singleton().getChannelByName(message.getParams()[0]);
	Server::Singleton().addClientToChannel(client, chan);
	Server::Singleton().sendMsg(client, ":"+ client->getNickName() + " JOIN " + message.getParams()[0] + "\r\n");
}

bool	ChnlJoinCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	struct pollfd *fd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(fd);
	if (!client->isVerified() /*|| (_client->isVerified() && (_client->getNickName().empty() || _client->getRealName().empty()))*/)
	{
		Server::Singleton().sendMsg(client, "ERR_NOTREGISTERED :You have not registered\r\n");
		return false;
	}
	if (msg.getParams().size() < 1)
	{
		Server::Singleton().sendMsg(client, "ERR_NEEDMOREPARAMS JOIN :Not enough parameters\r\n");
		return false;
	}
	if (Server::Singleton().getChannelByName(msg.getParams()[0]) == 0)
	{
		Server::Singleton() += msg.getParams()[0];
	}
	Channel *chan = Server::Singleton().getChannelByName(msg.getParams()[0]);
	if (chan->getClientsFromChannel()->size() == chan->getLimit())
	{
		Server::Singleton().sendMsg(client, "ERR_CHANNELISFULL :Channel is full, sorry\r\n");
		return false;
	}
	if (chan->getModes()->inviteOnly == true) //check channel permissions;
	{
		Server::Singleton().sendMsg(client, "ERR_INVITEONLYCHAN :Channel is Invite Only\r\n");
		return false;
	}
	execute(client, msg);
	return true;
}
