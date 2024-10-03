#pragma once
#include "Client.hpp"

class Channel{
    private:
        std::vector<Client*>      _clients;
        std::vector<Client*>      _admins;
        std::string               _name;
    public:
        Channel& operator+=(Client const& cli);
        Channel& operator-=(Client const& cli);
        std::vector<Client*>  *getClientsFromChannel();
        Client*               getClientByNickName(std::string name);
        Client*               getClientByRealName(std::string name);
        struct pollfd         *getClientFd(Client* client);
        std::string           getChannelName();
        std::vector<Client*>  *getAdmins();
        void                  setName(const std::string &name);
    Channel();
    ~Channel();
};