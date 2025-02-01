#include "../includes/irc.hpp"


/// TODO:
// **1 Ao entrar no servidor recebe logo esta mensagem: ">> ircserv 462  :You may not reregiste"
int	Server::nick( Client &client ){
	std::string	forbiddenChars[11] = {",*.?!@\"$# "};

	try {
		//client.verifyClientRegistered();	**1
        if (client.args.size() < 2)
	        throw (431);
        client.verifyValidNick();
        if (std::strcmp(client.getNick().c_str(), client.args.at(1).c_str()) == 0)
		    return (0);
        checkClientNickExists(client.args.at(1));

        std::cout << "Changing client's name from " << client.getNick() << " to " << client.args.at(1) <<  std::endl;
		buf(client, 0, client.args.at(1), "NICK");
		client.setNick(client.args[1]);
    }
	catch (int e)
	{
		buf(client, e, "", "NICK");
		//buf(client, (int)e.what(), "", "NICK");
	}
	return (0);
}

int	Server::user( Client &client )
{
    try {
        if (client.args.size() < 4)
		    throw (461);
        //verifyClientNotRegistered(client);
        if (client.args.at(2) != "0" && client.args.at(3) != "*")
		    return (0); //necessary?
            
        std::cout << "setting " << client.getNick() << "'s user to " << client.args.at(1) << " and real name to " << client.args.at(4) << std::endl;
		client.setUser(client.args.at(1));

		//must be the last parameter because it may contain SPACE (' ', 0x20) characters, and should be prefixed with a colon (:) if required
		client.setRealname(client.args.at(4));
		buf(client, 0, client.args.at(1) + " 0 * " + client.args.at(4), "USER");
    }
    catch (std::exception &e){
		buf(client, 2, "", "NICK");
		//buf(client, (int)e.what(), "", "USER");
	}
	return 0;
}

/// TODO:
//  /pass sem parametros dispara erro
//  antes de por a pass devia dar display de prompt para por a pass
//  apos inserir passe convem informar ao cliente que fez login successful
int	Server::pass( Client &client )
{
	try {
		if (client.args.at(1) != this->password)
			throw (464);
		//verifyClientNotRegistered(client);
		client.setRegisterd(true);
		client.setId(client.createId());

	}
	catch (int e){
		buf(client, e, "", "NICK");
		//buf(client, (int)e.what(), "", "PASS");
	}
	return (0);
}

int	Server::cap( Client &client )
{
	(void) client;
	return (0);
}