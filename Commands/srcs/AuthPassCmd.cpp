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
	client->setVerified();
}

bool AuthPassCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	if (message.getParams()[1].empty())
	{
		Server::Singleton().sendMsg(client, "ERR_NONICKNAMEGIVEN :No nickname given\r\n"); //cambiar error
		return false;
	}
	if (message.getParams()[1] != Server::Singleton().getPasswd())
	{
		Server::Singleton().sendMsg(client, "ERR_NONICKNAMEGIVEN :No nickname given\r\n"); //cambiar error
		return false;
	}
	execute(client, message);
	return true;
}
