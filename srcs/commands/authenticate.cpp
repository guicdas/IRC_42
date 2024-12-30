#include "../includes/irc.hpp"

static void	verifyClientRegistered( t_client &c ){
	if (c.registered == 0)
        throw (1); //mudar numero
}

static void	verifyValidNick( t_client &c ){
    std::string	forbiddenChars[11] = {",*.?!@\"$# "};

// falta o ':' que so pode estar em primeiro
    if (std::strpbrk(c.args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		throw (432);
}

int	Server::nick( t_client &client ){
	std::string	forbiddenChars[11] = {",*.?!@\"$# "};

	try {
		verifyClientRegistered(client);
        if (client.args.size() < 2)
	        throw (431);
        verifyValidNick(client);
        if (std::strcmp(client.nickname.c_str(), client.args.at(1).c_str()) == 0)
		    return (0);
        if (checkClientNickExists(client.args.at(1)) == 1)
		    throw (433);

        std::cout << "Changing client's name from " << client.nickname << " to " << client.args.at(1) <<  std::endl;
		buf(client, 0, client.args.at(1), "NICK");
		client.nickname = client.args.at(1);
    }
	catch (std::exception &e){
		buf(client, (int)e.what(), "", "NICK");
	}
	return (0);
}

static void	verifyClientNotRegistered( t_client &c ){
	if (c.registered == 1)
        throw (462); //mudar numero
}

int	Server::user( t_client &client )
{
    try {
        if (client.args.size() < 4)
		    throw (461);
        verifyClientNotRegistered(client);
        if (client.args.at(2) != "0" && client.args.at(3) != "*")
		    return (0); //necessary?
            
        std::cout << "setting " << client.nickname << "'s user to " << client.args.at(1) << " and real name to " << client.args.at(4) << std::endl;
		client.username = client.args.at(1);

		//must be the last parameter because it may contain SPACE (' ', 0x20) characters, and should be prefixed with a colon (:) if required
		client.realname = client.args.at(4);
		buf(client, 0, client.args.at(1) + " 0 * " + client.args.at(4), "USER");
    }
    catch (std::exception &e){
		buf(client, (int)e.what(), "", "USER");
	}
}

int	Server::pass( t_client &client )
{
	try {
		if (client.args.at(1) != this->password)
			throw (464);
		verifyClientNotRegistered(client);
		client.registered++;
	}
	catch (std::exception &e){
		buf(client, (int)e.what(), "", "PASS");
	}
	return (0);
}

int	Server::cap( t_client &client )
{
	(void) client;
	return (0);
}