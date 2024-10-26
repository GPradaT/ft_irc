#include "../includes/AuthUserCmd.hpp"
#include "../../Core/includes/Server.hpp"

AuthUserCmd::AuthUserCmd()
{
}

AuthUserCmd::~AuthUserCmd()
{
}

void AuthUserCmd::execute(Client *client, IRCMessage const&message)
{
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	client->setReal(str);
	if (!client->isVerified() && client->getNickName() != "")
		client->setVerified();
	std::cout << "UserName: " << client->getRealName() << " | NickName: " << client->getNickName() << std::endl;
}

bool AuthUserCmd::validate(IRCMessage const&message)
{
	struct pollfd *cliFd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(cliFd);
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	if (!client->correctPwd() && (!Server::Singleton().getPasswd().empty() || Server::Singleton().getPasswd() != ""))
	{
		Server::Singleton().sendMsg(client, "ERR_PASSWDMISMATCH :Wrong Password\r\n");
		Server::Singleton() -= client;
		return false;
	}
	if ((message.getParams().size() != 3 && message.getTrailing().empty()) ||
		(message.getParams().size() == 3 && message.getTrailing().empty()) ||
		(message.getParams().size() != 3 && !message.getTrailing().empty()))
	{
		Server::Singleton().sendMsg(client, "ERR_NEEDMOREPARAMS :User command needs more params\r\n");
		return false;
	}
	if (client->isVerified())
	{
		Server::Singleton().sendMsg(client, "ERR_ALREADYREGISTERED :You are already registered\r\n");
		return false;
	}
	execute(client, message);
	return true;
}
