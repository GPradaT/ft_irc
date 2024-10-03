#pragma once

#include <sstream>
#include <iostream>
#include <string>

#include <vector>

class	IRCMessage
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::string					_trailing;
		std::vector<std::string>	_params;

	public:
		IRCMessage(const std::string &buffer);
		~IRCMessage();

		void	setPrefix(const std::string &prefix);
		void	setCommand(const std::string &command);
		void	setParams(std::vector<std::string> &params);
		void	setTrailing(std::stringstream &new_buffer, const std::string &trailing);

		//std::string	&getPrefix() const;
		//std::string	&getCommand() const;
		//std::vector	&getParams() const;
		//std::string	&getTrailing() const;

		void		print() const;
};
