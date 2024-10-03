// C++ program to show the example of server application in
// socket programming
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>

#include "../includes/IRCMessage.hpp"

int main(int argc, char *argv[])
{
	try {
		IRCMessage::validInput(argc, argv);
	} catch (...) {
		return 1;
	}
	// creating socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	std::vector<struct pollfd> _fds;
	int polVal = 0;
	bool endServer = false;
	int counter = 0;
	int tmp_fd;
	// specifying the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8800);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	// binding socket.
	bind(serverSocket, (struct sockaddr*)&serverAddress,
		sizeof(serverAddress));

	// listening to the assigned socket
	listen(serverSocket, 5);
	//memset(fds, 0 , sizeof(fds));
	struct pollfd server;
	_fds.push_back(server);
	_fds[0].fd = serverSocket;
	_fds[0].events = POLLIN;

	while (!endServer){
		polVal = poll(&_fds[0], _fds.size(), -1);
		_fds[0].events = POLLIN;
		counter = _fds.size();
		for (int i = 0; i < counter; i++)
		{
			if(_fds[i].revents == 0)
			    continue;
			if (_fds[i].fd == serverSocket)
			{
				tmp_fd = accept(serverSocket, nullptr, nullptr);
				if (tmp_fd < 0)
					break;
				struct pollfd clientfd;
				clientfd.fd = tmp_fd;
				clientfd.events = POLLIN;
				_fds.push_back(clientfd);
				_fds[0].events = POLLOUT;
			}
			else
			{
				char buffer[1024] = { 0 };
				int recVal = 0;
				recVal = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
				std::cout << buffer << buffer[std::strlen(buffer) - 2] << std::endl;
				std::string str = buffer;
				IRCMessage message(str);
				//if (str.find("JOIN") != std::string::npos)
				//{
				//    if (!jaumein)
				//    {
				//        std::string response = ":Jaume!Jaume@localhost JOIN #test\r\n";
				//        send(_fds[i].fd,response.c_str(), response.length(), 0);
				//        jaumein = true;
				//    }
				//    else{
				//        std::string response = ":PEPITO!PEPITO@localhost JOIN #test\r\n";
				//        send(_fds[i].fd,response.c_str(), response.length(), 0);
				//    }
				//}
				//if (str.find("PRIVMSG") != std::string::npos){
				//    std::string response = ":Jaume!Jaume@localhost PRIVMSG #test :Hi everyone!\r\n";
				//    send(_fds[i].fd,response.c_str(), response.length(), 0);
				//}
				memset(buffer,0,1024);
			}
		}
	}
	// closing the socket.
	close(serverSocket);

	return 0;
}
