#pragma once

#include <string>

class	IRCMessage
{
  private:
    std::string	_prefix;
    std::string	_command;
    std::string	*_params;
    std::string	_trailing;

  public:
    IRCMessage(int argc, char **argv);
    ~IRCMessage();
    void	setPrefix(const std::string &prefix);
    void	setCommand(const std::string &command);
    void	setParams();
    void	setTrailing(const std::string &trailing);
    
    std::string	getPrefix() const;
    std::string	getCommand() const;
    std::string	*getParams() const;
    std::string	getTrailing() const;

    void        print() const;
    
};
