#pragma once

#include <string>

Class  Client
{
  private:
    std::string  _nickname;
    std::string  _username;
    int          fd;

  public:
    std::string  getNickname() const;
    std::string  getUsername() const;
};
