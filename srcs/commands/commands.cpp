#include "../includes/irc.hpp"

int	Server::join( Client &client ){
	std::string	forbiddenChars[4] = {",\x07 "};

	if (client.args.at(1)[0] != '#' && client.args.at(1)[0] != '&')
		buf(client, 403, client.args.at(1), "");
	else if (std::strpbrk(client.args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		buf(client, 403, client.args.at(1), "");
	else
	{
		//if (checkChannelNameExists(client.args.at(1)) == 1)
		{
			//if (isClientInChannel(client, getChannel(client.args.at(1))))
			//	return (0); // verif se n é erro
			addUserToChannel(client, getChannel(client.args.at(1)));
			std::cout << "client " << client.getNick() << " joined " << client.args.at(1) << std::endl;
			buf(client, 0, client.args.at(1) + "* :realname\n", "JOIN");
		}
		//else
		{
			std::cout << "creating channel " << client.args.at(1) << ENDL;
			Channel ch;
			ch.name = client.args.at(1);
			addUserToChannel(client, &ch);
		}

		buf(client, 0, client.args.at(1) + "* :realname\n", "JOIN");
        //Server::mode(client);
		/*
        MODE #canalasd
        << WHO #canalasd %chtsunfra,152
        */
	}
	return (0);
}

int	Server::list( Client &client ){
	std::cout << "client " << client.getNick() << " listing all channels: " << std::endl;

	for (std::vector< Channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		Channel &ch = *it;
		client.buffer = ch.name + "\t\t" + ch.topic + ".\n";
		clientWrite(client);
	}
	return (0);
}

int	Server::part( Client &client )
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
			if (channelName == channel.name)
			{
				client.channels.erase(itCh);
				eraseClientFromChannel(client, getChannel(channel.name));
				buf(client, 0, ": Leaving", "PART");
			}
		}
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
		checkChannelNameExists(client.args.at(1));
		checkClientNickExists(client.args.at(2));
		checkClientOp(client, getChannel(client.args.at(1)));
		checkClientInChannel(getClient(client.args.at(2)), getChannel(client.args.at(1)));
		
		Client &c = getClient(client.args.at(2));
		close(c.getFd());
		FD_CLR(c.getFd(), &this->fdList);
		eraseClientFromAllChannels(c);
	}
	catch (std::exception &e){
		buf(client, 2, "", "NICK");
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
			if (client.args.at(1) == channel.name)
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
		buf(client, 2, "", "NICK");
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

int	Server::invite( Client &client){
	PRINT_COLOR(RED, client.args[0]);
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