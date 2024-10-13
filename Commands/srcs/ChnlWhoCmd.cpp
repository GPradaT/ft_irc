#include "../includes/ChnlWhoCmd.hpp"
#include "../../Core/includes/Server.hpp"

ChnlWhoCmd::ChnlWhoCmd()
{

}

ChnlWhoCmd::~ChnlWhoCmd()
{
	
}

void	ChnlWhoCmd::execute(Client *client, IRCMessage const&message)
{
	//send message to all clients in the channel
	//send message to the client that joined the channel
}

bool	ChnlWhoCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	struct pollfd *fd = Server::Singleton().getCurrentFd();
	Client *client = Server::Singleton().getClientByFd(fd);
	Channel *chan = Server::Singleton().getChannelByName(msg.getParams()[0]);
	for	(int i = 0; i < chan->getClientsFromChannel()->size(); i++)
	{
		Client *to = *getAt((*chan->getClientsFromChannel()), i);
		chan->sendToAll(":" + client->getServer() + " 352 " + client->getNickName() + " " + msg.getParams()[0] + " " + to->getRealName() + " " + to->getHostName() + " " + to->getServer() + " " + to->getNickName() + " H: 0 " + to->getRealName() + "\r\n");
	}
	chan->sendToAll(":" + client->getServer() + " 315 " + client->getNickName() + " " + msg.getParams()[0] + " :End of /WHO list" + "\r\n");
	execute(client, msg);
	return true;
}
