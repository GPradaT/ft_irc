#include "AuthNickCmd.hpp"

AuthNickCmd::AuthNickCmd()
{
}

AuthNickCmd::~AuthNickCmd()
{
}

void NickCommand::execute(Server *server, Client *client)
{
    std::string newNick = message.getParams()[0];

    // Lógica para cambiar el nickname del cliente
    if (server->isNickAvailable(newNick))
    {
        client->setNick(newNick);
        // Informar a otros clientes si es necesario
    }
    else
    {
		// donde esta el mensaje, podemos poner o el núemro del error o el titulo del error
		// y hacer un enum con los errores que tengamos nosotros definidos, eso sí, con el mismo número
		// que indica el IRC, los he dejado apuntados en cada uno de los .hpp de los errores que devuelve
		// el servidor por cada comando de los que tendremos que gestionar
        server->sendMsg(client, "Error: Nickname ya está en uso.");
    }
}

bool NickCommand::validate(IRCMessage &message)
{
    if (!AuthenticationCommand::validate(message))
        return false;

    // Validaciones adicionales específicas de NICK
    // Por ejemplo, verificar que el nickname cumpla con las reglas
    return isValidNick(message.getParams()[0]);
}
