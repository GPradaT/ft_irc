#pragma once

#include "ICommand.hpp"

class	AuthenticationCommand : public ICommand
{
	protected:
		bool		_isClientRegistered(Client *Client);

	public:
		virtual	~AuthenticationCommand();

		virtual void	execute(Server *Server, Client *Client) = 0;
		virtual bool	validate(IRCMessage &Message) = 0;
	// Comandos de autenticación: PASS, NICK, USER
};
