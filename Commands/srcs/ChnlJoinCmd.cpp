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
	std::string str = message.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	Server::Singleton().sendMsg(_client, ":"+ _client->getNickName() + " JOIN " + str + "\r\n");
	Channel *chan = Server::Singleton().getChannelByName(str);
	if (chan){
		for (int i = 0; i < chan->getClientsFromChannel()->size(); i++)
		{
			Client *to = (*chan->getClientsFromChannel())[i];
			chan->sendToAll(":Server 353 " + client->getNickName() + " = " + chan->getChannelName() + " :@" + to->getNickName() + "\r\n");
		}
		chan->sendToAll(":Server 366 " + client->getNickName() + " " + chan->getChannelName() + " :End of /NAMES list\r\n");
	}
}

bool	ChnlJoinCmd::validate(IRCMessage const&msg)
{
	//validate the command
	//return false if the command is invalid
	_clFd = Server::Singleton().getCurrentFd();
	_client = Server::Singleton().getClientByFd(_clFd);
	std::string str = msg.getParams()[0];
	str.erase(std::remove(str.begin (), str.end (), '\r'), str.end());
	str.erase(std::remove(str.begin (), str.end (), '\n'), str.end());
	// //std::cout << "el nombre del canal es -> [" << str <<  "] y el size de la string es -> " << str.length() << std::endl;
	if (!_client->isVerified() /*|| (_client->isVerified() && (_client->getNickName().empty() || _client->getRealName().empty()))*/)
	{
		Server::Singleton().sendMsg(_client, ":" + _client->getNickName() + " 451 :You have not registered\r\n");
		return false;
	}
	if (msg.getParams().size() < 1 || str.length() <= 1)
	{
		Server::Singleton().sendMsg(_client, ":" + _client->getNickName() + " 461 JOIN :Not enough parameters\r\n");
		return false;
	}
	if (Server::Singleton().getChannelByName(str) == 0)
	{
		Server::Singleton() += str;
	}
	else
	{
		Channel *chan = Server::Singleton().getChannelByName(str);
		std::string key = "";
		if (msg.getParams().size() > 1)
			key = msg.getParams()[1];
		if (chan->getKey() != key)
		{
			Server::Singleton().sendMsg(_client, " 475 " + _client->getNickName() + " " + str + ":Cannot join channel (+k)\r\n");
			return false;
		}
	}
	if (Server::Singleton().getChannelByName(str)->getModes()->inviteOnly == false) //check channel permissions;
	{
		//en caso de que se mueva el cliente, lo quitamos del channel en el que este y luego lo anadimos al nuevo
		/*Channel *current = Server::Singleton().getChannelByClient(_client);
		if (current != 0)
			*current -= _client;*/
		Channel *chan = Server::Singleton().getChannelByName(str);
		if (chan->getClientByNickName(_client->getNickName()) == 0)
			*chan += _client;
		// //std::cout << "hay esta cantidad de cleintes en el chan ---> " << chan->getClientsFromChannel()->size() << std::endl;
	}
	else
	{
		/* Esta numeric reply nose si deberia usarse.
        471     ERR_CHANNELISFULL
                        "<channel> :Cannot join channel (+l)"
		Y a esta me espero a la lista de invitados.
        473     ERR_INVITEONLYCHAN
                        "<channel> :Cannot join channel (+i)"
		*/
		Server::Singleton().sendMsg(_client, "ERR_RESTRICTED :Your connection is restricted!\r\n");
		return false;
	}
	execute(_client, msg);
	return true;
}
