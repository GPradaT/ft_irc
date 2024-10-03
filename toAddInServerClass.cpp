
unsigned short	_port;
std::string	_password;

void	Server::setPort(const char *str)
{
  std::string	    value = str;
  std::stringstream buffer(value);

  unsigned short port = 0;
  try {
    buffer >> port;
    _port = port;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::cout << "Funciona" << std::endl;
  }
  std::cout << "Valor de _port -> " << _port << std::endl;
  std::cout << "Valor de port -> " << port << std::endl;
}

void	Server::setPassword(const char *pass)
{
  _password = _pass;
}

void	Server::validInput(int argc, char *argv[])
{
	if (argc == 3)
	{
		Server::Sngl().setPort(argv[1]);
		Server::Sngl().setPassword(argv[2]);
	}
	else
		throw std::invalid_argument(USAGE);
}
