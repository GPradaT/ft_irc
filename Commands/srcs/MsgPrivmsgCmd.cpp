#include "../includes/MsgPrivmsgCmd.hpp"

MsgPrivmsgCmd::MsgPrivmsgCmd() {}

MsgPrivmsgCmd::~MsgPrivmsgCmd() {}

bool MsgPrivmsgCmd::validate(IRCMessage const &message)
{
    Client *client = Server::Singleton().getClientByFd(Server::Singleton().getCurrentFd());
    std::string target = message.getParams()[0];

    if (message.getParams().empty())
    {
        Server::Singleton().sendMsg(client, "411 ERR_NORECIPIENT :No recipient given (PRIVMSG)\r\n");
        return false;
    }
    if (message.getParams().size() < 2)
    {
        Server::Singleton().sendMsg(client, "412 ERR_NOTEXTTOSEND :No text to send\r\n");
        return false;
    }

    return true;

}

void MsgPrivmsgCmd::execute(Client *client, IRCMessage const &message)
{
    if (!validate(message))
    {
        return;
    }

    std::string targetNick = message.getParams()[0];
    std::string msgContent = message.getParams()[1];
    std::cout << "Mensaje privado de " << client->getNickName() << " a " << targetNick << ": " << msgContent << std::endl;

    Client *targetClient = Server::Singleton().getClientByNickName(targetNick);
    if (targetClient)
    {
        std::string fullMsg = ":" + client->getNickName() + " PRIVMSG " + targetNick + " :" + msgContent + "\r\n";
        Server::Singleton().sendMsg(targetClient, fullMsg);
    }
    else
    {
        Server::Singleton().sendMsg(client, "401 ERR_NOSUCHNICK :No such nick/channel\r\n");
    }
}