#pragma once

#include "../../Headers.h"

class	IRCMessage
{
	private:
		unsigned short				_port;
		std::string					_prefix;
		std::string					_command;
		std::string					_trailing;
		std::vector<std::string>	_params;
		std::string					_outputCommand;

	public:
		IRCMessage(const std::string &buffer);
		~IRCMessage();

		void	setPrefix(const std::string &prefix);
		void	setCommand(const std::string &command);
		void	setParams(std::vector<std::string> &params);
		void	setTrailing(std::stringstream &new_buffer, const std::string &trailing);
		void	setPort(const std::string &port);

		static void	validInput(int argc, char *argv[]);


		std::string					&getPrefix() const;
		std::string					&getCommand() const;
		std::vector<std::string>	&getParams() const;
		std::string					&getTrailing() const;

		void		print() const;
};
