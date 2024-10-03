#include "../includes/IRCMessage.hpp"

void  IRCMessage::setPrefix(const std::string &prefix)
{
	_prefix = prefix.substr(1);
}

void  IRCMessage::setCommand(const std::string &command)
{
	_command = command;
}

void	IRCMessage::setParams(std::vector<std::string> &params)
{
	_params = params;
}

void	IRCMessage::setTrailing(std::stringstream &new_buffer, const std::string &trailing)
{
	std::string	temp;
	new_buffer >> temp;
	_trailing = trailing.substr(1) + " " + temp;
	while (new_buffer >> temp)
		_trailing += " " + temp;
}

IRCMessage::IRCMessage(const std::string &buffer)
{
	std::stringstream	new_buffer(buffer);
	std::string	token;
	while (new_buffer >> token)
	{
		if (token[0] == ':')
		{
			setTrailing(new_buffer, token);
			break;
		}
		_params.push_back(token);
	}
}
IRCMessage::~IRCMessage() {}
