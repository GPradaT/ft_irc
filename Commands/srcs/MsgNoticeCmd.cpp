#include "../includes/MsgNoticeCmd.hpp"

MsgNoticeCmd::MsgNoticeCmd()
{
}

MsgNoticeCmd::~MsgNoticeCmd()
{
}

void	MsgNoticeCmd::execute()
{

	//send message to all clients in the channel
	//send message to the client that joined the channel
}

bool	MsgNoticeCmd::validate(IRCMessage &msg)
{
	if (msg.getParams().size() < 2)
	{
		Server::Singleton().sendMsg(_client, " :Not enough parameters\r\n");
		return false;
	}
	if (msg.getParams()[1].empty())
	{
		Server::Singleton().sendMsg(_client, "ERR_NOTEXTTOSEND :No text to send\r\n");
		return false;
	}
	//validate the command
	//return false if the command is invalid
	return true;
}
