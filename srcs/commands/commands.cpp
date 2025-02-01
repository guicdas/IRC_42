#include "../includes/irc.hpp"

/// TODO:
//	So pode correr commandos normais qunado estiver autenticado
int	Server::join( Client &client ){
	try
	{
		std::string	forbiddenChars[4] = {",\x07 "};
		std::string channelName = client.args[1];

		if (channelName[0] != '#' && channelName[0] != '&')
			buf(client, 403, client.args.at(1), "");
		else if (std::strpbrk(channelName.c_str(), forbiddenChars->c_str()) != NULL)
			buf(client, 403, client.args.at(1), "");
		else
		{
			if (doesChannelNameExist(channelName))
			{
				if (isClientInChannel(client.getNick(), channelName))
					throw(443);	// NO SPECIFIC ERR_CODE
				addUserToChannel(client, getChannel(channelName));
				PRINT_COLOR(GREEN, "client " + client.getNick() + " joined " + channelName);
				buf(client, 1001, client.args[1], "JOIN");
			}
			else
			{
				PRINT_COLOR(GREEN, "creating channel " + channelName);
				Channel ch(channelName);
				addUserToChannel(client, &ch);	// Should be added as operator
			}

			buf(client, 1001, client.args[1], "JOIN");
			//Server::mode(client);
			/*
			MODE #canalasd
			<< WHO #canalasd %chtsunfra,152
			*/
		}
	}
	catch(int e)
	{
		buf(client, e, "", "JOIN");
	}
	return (0);
}

int	Server::list( Client &client ){
	std::cout << "client " << client.getNick() << " listing all channels: " << std::endl;

	for (std::vector< Channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		Channel &ch = *it;
		client.buffer = ch.getName() + "\t\t" + ch.getTopic() + ".\n";
		clientWrite(client);
	}
	return (0);
}

int	Server::part( Client &client )
{
	try
	{
		std::istringstream	channels(client.args.at(1));
		std::string	channelName;

		while (std::getline(channels, channelName, ','))
		{
			checkChannelNameExists(channelName);
			checkClientInChannel(client, getChannel(channelName)); // 441

			for (std::vector< Channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
			{
				Channel &channel = *itCh;
				if (channelName == channel.getName())
				{
					client.channels.erase(itCh);
					eraseClientFromChannel(client, getChannel(channel.getName()));
					buf(client, 0, ": Leaving", "PART");
				}
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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
		Client		kicked		= getClient(kickedName);
		std::string channelName = client.args[1];
		Channel		*channel 	= getChannel(channelName);

		//checkClientOp(client, channel);
		checkClientInChannel(kicked, channel);
		
		// O CLIENTE É KICKED DO CANAL NAO DO SERVIDOR

		//close(kicked.getFd());
		//FD_CLR(kicked.getFd(), &this->fdList);
		//eraseClientFromAllChannels(kicked);

		// NOTIFICAR A TODOS OS CLIENTES COM ESTA MENSAGEM
		eraseClientFromChannel(kicked, channel);
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

		std::cout << "client " << client.getNick() << " sending \"" << client.args.at(1) << "\"" << std::endl;
		for (std::vector< Channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
		{
			Channel &channel = *itCh;
			if (client.args.at(1) == channel.getName())
			{
				sendMsgToChannel(channel, client.args.at(1), client.args.at(2));
				return (0); // verif return
			}
		}
		checkChannelNameExists(client.args.at(2)); // 324
		checkClientNickExists(client.args.at(1)); // SE Nao existir da erro?
		sendMsgToUser(client.args.at(1), client.args.at(2));
	}
	catch (std::exception &e){
		buf(client, 2, "", "PRIVMSG");
		//buf(client, (int)e.what(), "", "PRIVMSG");
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
		if (isClientInChannel(invitedClient, channelName))
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