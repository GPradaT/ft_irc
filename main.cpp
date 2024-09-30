#include "IRCMessage.hpp"

int	main(int argc, char **argv)
{
	if (argc == 4)
	{
		IRCMessage  Irc(argc, argv);
		Irc.print();
	}
	return 0;
}
