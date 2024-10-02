#include "../includes/IRCMessage.hpp"

void  IRCMessage::setPrefix(const std::string &prefix)
{
	_prefix = prefix;
}

void  IRCMessage::setCommand(const std::string &command)
{
	_command = command;
}

IRCMessage::IRCMessage(const std::string &buffer)
{
	std::sßtringstream	new_buffer(buffer);
	std::string	token;
	while (new_buffer >> token)
	{
		std::cout << "Valor del token actual -> " << token << std::endl;
	}
}
IRCMessage::~IRCMessage() {} 
