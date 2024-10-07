#pragma once

#include "../../Core/includes/Channel.hpp"
#include "../../Core/includes/Client.hpp"
#include "../../Core/includes/IRCMessage.hpp"

class	ICommand
{
	public:
		ICommand() {};
		virtual ~ICommand() {};

		virtual void	execute(Client *client, IRCMessage const&message) = 0;
		virtual bool	validate(IRCMessage const& message) = 0;
};

class ChannelCommand : public ICommand
{
    // Comandos de canal: JOIN, PART, MODE, TOPIC, INVITE, KICK
};

class MessagingCommand : public ICommand
{
    // Comandos de mensajería: PRIVMSG, NOTICE
};

//	el comando QUIT alomejor no quieres hacer clase derivada pero estaría bien
//	porqué se derivaría de esta clase que sería la clase base de otros comandos como PONG

class ConnectionCommand : public ICommand
{
    // Comandos de conexión: QUIT
};



