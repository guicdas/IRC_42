#include "../includes/irc.hpp"

int	Server::join( t_client &client ){
	std::string	forbiddenChars[4] = {",\x07 "};

	if (client.args.at(1)[0] != '#' && client.args.at(1)[0] != '&')
		buf(client, 403, client.args.at(1), "");
	else if (std::strpbrk(client.args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		buf(client, 403, client.args.at(1), "");
	else
	{
		if (checkChannelNameExists(client.args.at(1)) == 1)
		{
			if (isClientInChannel(client, getChannel(client.args.at(1))))
				return (0); // verif se n é erro
			addUserToChannel(client, getChannel(client.args.at(1)));
			std::cout << "client " << client.nickname << " joined " << client.args.at(1) << std::endl;
			buf(client, 0, client.args.at(1) + "* :realname\n", "JOIN");
		}
		else
		{
			std::cout << "creating channel " << client.args.at(1) << ENDL;
			t_channel ch;
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

int	Server::list( t_client &client ){
	std::cout << "client " << client.nickname << " listing all channels: " << std::endl;

	for (std::vector< t_channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		t_channel &ch = *it;
		client.buffer = ch.name + "\t\t" + ch.topic + ".\n";
		clientWrite(client);
	}
	return (0);
}

int	Server::part( t_client &client )
{
	std::istringstream	channels(client.args.at(1));
	std::string	channelName;

	while (std::getline(channels, channelName, ','))
	{
		checkChannelNameExists(channelName);
		isClientInChannel(client, getChannel(channelName)); // 441

		for (std::vector< t_channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
		{
			t_channel &channel = *itCh;
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

int	Server::quit( t_client &client ){
	buf(client, 0 , client.args.at(2), "QUIT");
	clientWrite(client);

	close(client.fd);
	FD_CLR(client.fd, &this->fdList);

	eraseClientFromAllChannels(client);
	
	return (1);
}

int	Server::who( t_client &client ){
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

int	Server::kick( t_client &client ){
	try{
		checkChannelNameExists(client.args.at(1));
		checkClientNickExists(client.args.at(2));
		isClientOp(client, getChannel(client.args.at(1)));
		isClientInChannel(getClient(client.args.at(2)), getChannel(client.args.at(1)));
		
		t_client &c = getClient(client.args.at(2));
		close(c.fd);
		FD_CLR(c.fd, &this->fdList);
		eraseClientFromAllChannels(c);
	}
	catch (std::exception &e){
		buf(client, (int)e.what(), "", "KICK");
	}
	return (0);
}


int	Server::privmsg( t_client &client ){
	try{
		if (client.args.size() < 2)
			throw (411);
		if (client.args.size() < 2)
			throw (412);

		std::cout << "client " << client.nickname << " sending \"" << client.args.at(1) << "\"" << std::endl;
		for (std::vector< t_channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
		{
			t_channel &channel = *itCh;
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
		buf(client, (int)e.what(), "", "PRIVMSG");
	}
	return (0);
}
