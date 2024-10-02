#include "IRCMessage.hpp"

void	IRCMessage::setPrefix(const std::string &prefix)
{
	_prefix = prefix;
}

void	IRCMessage::setCommand(const std::string &command)
{
	_command = command;
}

IRCMessage::IRCMessage(int argc, char **argv)
{
	if (argc == 4)
	{
		setPrefix(std::string(argv[1]));
		setCommand(std::string(argv[2]));
		setTrailing(std::string(argv[3]));
	}
}
