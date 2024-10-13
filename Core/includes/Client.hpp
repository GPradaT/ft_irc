#pragma once

#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <algorithm>
#include <deque>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class	Client
{
	private:
		std::string		_nick;
		std::string		_real;
		struct pollfd	*_fd;
		bool			_isOperator;
		bool			_isVerified;
		std::string 	_server;
		std::string		_host;
	public:
		bool			operator==(const Client &n2);

		struct pollfd	*getFd();
		std::string		getNickName();
		std::string		getHostName();
		std::string		getRealName();
		std::string		getServer();
		bool			isOperator();
		bool			isVerified();

		void			setVerified();
		void			setFd(struct pollfd *fd);
		void			setNick(const std::string &name);
		void			setReal(const std::string &name);
		void			setAdmin(bool admin);
		void			setServer(const std::string &name);
		void			setHost(const std::string &name);
	Client();
	~Client();
};