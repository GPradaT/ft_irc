#include "../includes/AuthPassCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthPassCmd::AuthPassCmd()
{
}

AuthPassCmd::~AuthPassCmd()
{
}

void AuthPassCmd::execute(Client *client, IRCMessage const&message)
{
	if (client->getNickName().empty())
		client->setNick(message.getParams()[1]);
	Server::Singleton().sendMsg(client, ":" + client->getNickName() + " NICK " + message.getParams()[1] + "\r\n");
	client->setNick(message.getParams()[1]);
}

bool AuthPassCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	if (message.getParams()[1].empty())
	{
		Server::Singleton().sendMsg(client, "ERR_NONICKNAMEGIVEN :No nickname given\r\n");
		return false;
	}
	execute(client, message);
	return true;
}
