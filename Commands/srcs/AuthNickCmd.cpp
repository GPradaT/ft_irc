#include "../includes/AuthNickCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthNickCmd::AuthNickCmd()
{
}

AuthNickCmd::~AuthNickCmd()
{
}

void AuthNickCmd::execute(Client *client, IRCMessage const&message)
{
	if (client->getNickName().empty())
		client->setNick(message.getParams()[1]);
	Server::Singleton().sendMsg(client, ":" + client->getNickName() + " NICK " + message.getParams()[1] + "\r\n");
	client->setNick(message.getParams()[1]);
}

bool AuthNickCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	/*if (!client->isVerified())
	{
		Server::Singleton().sendMsg(client, "ERROR :No password provided\r\n");
		return false;
	}*/
	if (message.getParams()[1].empty())
	{
		Server::Singleton().sendMsg(client, "ERR_NONICKNAMEGIVEN :No nickname given\r\n");
		return false;
	}
	if (false) //TO DO, implementar check de caracteres correctos
	{
		Server::Singleton().sendMsg(client, "ERR_ERRONEUSNICKNAME " + message.getParams()[0] + " :Erroneous nickname\r\n");
		return false;
	}
	if (Server::Singleton().getClientByNickName(message.getParams()[0]) != 0
		&& Server::Singleton().getClientByNickName(message.getParams()[0]) != client)
	{
		Server::Singleton().sendMsg(client, "ERR_NICKNAMEINUSE " + message.getParams()[0] + " :Nickname is already in use\r\n");
		return false;
	}

	execute(client, message);
	return true;
}
