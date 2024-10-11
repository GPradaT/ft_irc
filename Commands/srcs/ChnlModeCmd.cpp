#include "../includes/ChnlModeCmd.hpp"

ChnlModeCmd::ChnlModeCmd()
{
}

ChnlModeCmd::~ChnlModeCmd()
{
}

bool	ChnlModeCmd::validate(IRCMessage const&msg)
{
	struct pollfd	*_clFd = Server::Singleton().getCurrentFd();
	Client *_client = Server::Singleton().getClientByFd(_clFd);

	if (msg.getParams().size() < 2)
	{
		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
		return false;
	}
	if (msg.getParams()[0].empty())
	{
		Server::Singleton().sendMsg(_client, "ERR_NOSUCHNICK :No such nick/channel\r\n");
		return false;
	}
	//if (msg.getParams()[1][1] != '#') // Si es un modo de usuario
	//{
	//	std::string nick = msg.getParams()[1].substr(1);
	//	if (msg.getParams().size() < 3)
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
	//		return false;
	//	}
	//	if (nick.empty())
	//	{
	//		Server::Singleton().sendMsg(_client, "ERR_NOSUCHNICK :No such nick\r\n");
	//		return false;
	//	}
	//	if (Server::Singleton().getClientByNickName(nick))
	//	{
	//		std::string modes = msg.getParams()[2];
	//		size_t paramIndex = 3;
	//	}
	//}
	if (msg.getParams()[1][1] == '#') // Si es un modo de canal
	{
		std::string channelName = msg.getParams()[1];
		if (msg.getParams().size() < 3)
		{
			Server::Singleton().sendMsg(_client, "ERR_NEEDMOREPARAMS MODE :Not enough parameters\r\n");
			return false;
		}
		if (channelName.empty())
		{
			Server::Singleton().sendMsg(_client, "ERR_NOSUCHCHANNEL :No such channel\r\n");
			return false;
		}
		if (Server::Singleton().getChannelByName(channelName))
		{
			std::string modes = msg.getParams()[2];
			size_t paramIndex = 3;

		}
	}
	return true;
}

void	ChnlModeCmd::applyMode(char mode, bool adding, std::string &param, Client *client, Channel *channel)
{
	switch (mode)
	{
		case 'i':
			handleModeI(adding, channel);
			break;
		case 't':
			handleModeT(adding, channel);
			break;
		case 'k':
			handleModeK(adding, param, channel);
			break;
		case 'o':
			handleModeO(adding, client, channel);
			break;
		case 'l':
			handleModeL(adding, param, channel);
			break;
		default:
			Server::Singleton().sendMsg(client, "ERR_UNKNOWNMODE :Unknown mode\r\n");
			break;

	}
}
