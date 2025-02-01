#include "../includes/irc.hpp"

/// TODO:
//	So pode correr commandos normais qunado estiver autenticado
int	Server::join( Client &client ){
	try
	{
		std::string	forbiddenChars[4] = {",\x07 "};
		std::string channelName = client.args[1];

		if (channelName[0] != '#' && channelName[0] != '&')
			throw (1004);
		if (std::strpbrk(channelName.c_str(), forbiddenChars->c_str()) != NULL)
			throw (1004);
		if (doesChannelNameExist(channelName))
		{
			PRINT_COLOR(GREEN, "Client " + client.getNick() + " joining " + channelName);
			Channel &ch = getChannel(channelName);
			if (ch.isClientInChannel(client.getNick()))
				return (0);	//se o canal ja existe e o cliente ja la esta, nao faz nada
			ch.addClientToChannel(client);
		}
		else
		{
			PRINT_COLOR(GREEN, "Creating channel " + channelName);
			Channel ch(channelName);
			ch.addClientToChannel(client);
			ch.addOperatorToChannel(client);
			this->channels.push_back(ch);
		}
		buf(client, 1001, client.args[1], "JOIN");
	}
	catch(int e){
		buf(client, e, "", "JOIN");
	}
	return (0);
}

int	Server::list( Client &client ){
	PRINT_COLOR(GREEN, "client " + client.getNick() + " listing all channels:");

	for (std::vector< Channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		Channel &ch = *it;
		PRINT_COLOR(MAGENTA, ch.getName() + "\tTopic:" + ch.getTopic() + ".");
		client.buffer = ch.getName() + "\tTopic:" + ch.getTopic() + ".\n";
		clientWrite(client);
	}
	return (0);
}

// A Syntaxe e: part #canal1,#canal2 reason  -> a cena das virgulas ta implementada mas n deve tar a funcionar
int	Server::part( Client &client )
{
	try
	{
		std::istringstream	channels(client.args.at(1));
		std::string	channelName;

		while (std::getline(channels, channelName, ','))
		{
			PRINT_COLOR(BLUE, "line" + channelName);
			Channel &channel = getChannel(channelName);
			channel.checkClientInChannel(client); // throws 441

			for (std::vector< Channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
			{
				Channel &channel = *itCh;
				if (channelName == channel.getName())
				{
					client.channels.erase(itCh);
					channel.eraseClientFromChannel(client);
					if (client.args.size() > 2)// nao deve ser feito assim ->reason
						buf(client, 0, client.args.at(2), "PART");
					else
						buf(client, 0, ": Leaving", "PART"); //limpar?
					clientWrite(client);
				}
			}
		}
	}
	catch(int e){
		buf(client, e, "", "PART");
	}
	return (0);
}

int	Server::quit( Client &client ){
	buf(client, 0 , client.args.at(2), "QUIT");
	clientWrite(client);

	close(client.getFd());
	FD_CLR(client.getFd(), &this->fdList);

	eraseClientFromAllChannels(client);
	
	return (1);
}

int	Server::who( Client &client ){
	/*if (checkChannelNameExists(client.args.at(1)) == 1)
		listChannelMembers(client, getChannel(client.args.at(1)));
	else if (checkClientNickExists(client.args.at(1)) == 1)
	{
		buf(client, 0, "", "");
		clientWrite(client);
	}*/
	
	buf(client, 0, " :End of /who list", "");
	clientWrite(client);
	
	return (0);
}


int	Server::kick( Client &client ){
	try{
		if (client.args.size() < 2)
			throw (411);
		std::string kickedName	= client.args[2];
		Client		&kicked		= getClient(kickedName);
		std::string channelName = client.args[1];
		Channel		&channel 	= getChannel(channelName);

		//checkClientOp(client, channel);
		channel.checkClientInChannel(kicked);
		
		// O CLIENTE É KICKED DO CANAL NAO DO SERVIDOR

		//close(kicked.getFd());
		//FD_CLR(kicked.getFd(), &this->fdList);
		//eraseClientFromAllChannels(kicked);

		// NOTIFICAR A TODOS OS CLIENTES COM ESTA MENSAGEM
		//channel.eraseClientFromChannel(kicked);
		buf(client, 1003, channelName + " " + kickedName + ":Got kicked", "KICK");
	}
	catch (int e){
		buf(client, e, "", "KICK");
		//buf(client, (int)e.what(), "", "KICK");
	}
	return (0);
}


int	Server::privmsg( Client &client ){
	try{
		if (client.args.size() < 2)
			throw (411);
		if (client.args.size() < 2)
			throw (412);
		
		std::string messageToSend;
		for (size_t i = 2; i < client.args.size() ; i++){
			messageToSend += client.args.at(i);
			if (i < client.args.size() - 1)
				messageToSend += " ";
		}

		PRINT_COLOR(GREEN, "Client " + client.getNick() + " sending \"" + messageToSend + "\"");
		/*if (client.args.at(1)[0] == '#')
		{
			Channel &channel = getChannel(client.args.at(1));
			channel.sendMsgToChannel(client.args.at(1), client.args.at(2));
		}
		else*/
		{
			Client &c = getClient(client.args.at(1)); //throws 401
			PRINT_COLOR(RED, "client " + c.getNick() + " receiving \"" + messageToSend + "\"from " + client.getNick());
			c.buffer = client.getNick() + " :" + messageToSend + "\n";
			clientWrite(c);
		}
	}
	catch (int e){
		buf(client, e, "", "PRIVMSG");
	}
	return (0);
}


int	Server::topic( Client &client){
	if (client.args[0] != "")
		PRINT_COLOR(RED, client.args[0]);
	if (client.args[1] != "")
		PRINT_COLOR(RED, client.args[1]);
	/*try
	{
		if (client.args.size() < 2)
			throw (331);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}*/
	return (0);
}

/*
401
403
442
482
*/
int	Server::invite( Client &client){
	try
	{
		if (client.args.size() < 2)
			throw (331);
		std::string invitedClient = client.args[1];
		Client		invited = getClient(invitedClient);
		std::string channelName = client.args[2];
		//Channel		*channel	= getChannel(channelName);

		//checkClientOp(client, channel);
		Channel &channel = getChannel(channelName);
		if (channel.isClientInChannel(invitedClient))
			throw(443);
		// OTHER CHECKS
		buf(invited, 1002, channelName, "INVITE " + invitedClient);
		buf(client, 341, invitedClient + " " + channelName, "INVITE");
		
	}
	catch(int e)
	{
		buf(client, e, "", "INVITE");
	}
	return (0);
}