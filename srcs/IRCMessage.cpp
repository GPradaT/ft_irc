#include "IRCMessage.hpp"

void  IRCMessage::setPrefix(const std::string &prefix)
{
  _prefix = prefix;
}

void  IRCMessage::setCommand(const std::string &command)
{
  _command = command;
}

IRCMessage::IRCMessage(int argc, char **argv)
{
  for (int i = 1; i < argc; ++i)
  {

  }
}
