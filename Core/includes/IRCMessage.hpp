#pragma once

#include <vector>
#include <string>
#include <sstream>

class	IRCMessage
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::string					_trailing;
		std::vector<std::string>	_params;
		bool						_isValid;
		std::string					_rawMessage;

	public:
		IRCMessage(const std::string &buffer);
		~IRCMessage();

		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParams() const;
		const std::string				&getTrailing() const;

		bool							isValid() const;
		void							print() const;

	private:
		void							parseMessage(const std::string &buffer);

		bool							isValidCommand(const std::string &command);
		bool							isValidPrefix(const std::string &prefix);
		bool							isValidMiddleParam(const std::string &param);
		bool							isValidTrailingParam(const std::string &param);
};
