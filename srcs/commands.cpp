#include "../includes/irc.hpp"


int	Server::nick( t_client &client ){
	std::string	forbiddenChars[11] = {",*.?!@\"$# "};

	// falta o ':' que so pode estar em primeiro
	if (client.args.size() < 2)
		buf(client, 431, "", "");
	else if (std::strpbrk(client.args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		buf(client, 432, "", "");
	else if (std::strcmp(client.nickname.c_str(), client.args.at(1).c_str()) == 0)
		return ;
	else if (checkClientNickExists(client.args.at(1)) == 1)
		buf(client, 433, client.args.at(1), "");
	else
	{
		std::cout << "changing client's name from " << client.nickname << " to " << client.args.at(1) <<  std::endl;
		buf(client, 0, client.args.at(1), "NICK");
		client.nickname = client.args.at(1);
	}
	return (0);
}

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
				return ;
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
		if (checkChannelNameExists(channelName))
			buf(client, 403, "", "");
		else if (isClientInChannel(client, getChannel(channelName)))
			buf(client, 442, "", "");
		else
		{
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

int	Server::mode( t_client &client ){
	if (checkChannelNameExists(client.args.at(1)) == 1)
		buf(client, 403, client.args.at(1), "");
	else
		buf(client, 324, client.args.at(1), "");
	//If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned.
	// buf(client, 329, ); Servers MAY also return the RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.
	/* Servers MAY choose to hide sensitive information such as channel keys when sending the current modes. */

	return (0);
}

int	Server::who( t_client &client ){
	if (checkChannelNameExists(client.args.at(1)) == 1)
		listChannelMembers(client, getChannel(client.args.at(1)));
	else if (checkClientNickExists(client.args.at(1)) == 1)
	{
		buf(client, 0, "", "");
		clientWrite(client);
	}
	
	buf(client, 0, " :End of /who list", "");
	clientWrite(client);
	
	return (0);
}

int	Server::kick( t_client &client ){
	if (checkChannelNameExists(client.args.at(1)) == 1)
		buf(client, 403, client.args.at(1), "");
	else if (checkClientNickExists(client.args.at(2)) == 1)
		buf(client, 404, client.args.at(2), "");
	else if (isClientOp(client, getChannel(client.args.at(1))))
		buf(client, 482, client.args.at(1), "");
	else if (isClientInChannel(getClient(client.args.at(2)), getChannel(client.args.at(1))))
		buf(client, 441, client.args.at(1), "");
	else
	{
		t_client &c = getClient(client.args.at(2));
		close(c.fd);
		FD_CLR(c.fd, &this->fdList);
		eraseClientFromAllChannels(c);
	}
	return (0);
}


int	Server::privmsg( t_client &client )
{
	if (client.args.size() < 2)
		buf(client, 411, "", "");
	else if (client.args.size() < 3)
		buf(client, 412, "", "");
	else
	{
		std::cout << "client " << client.nickname << " sending \"" << client.args.at(1) << "\"" << std::endl;
		for (std::vector< t_channel >::iterator itCh = client.channels.begin(); itCh != client.channels.end(); itCh++)
		{
			t_channel &channel = *itCh;
			if (client.args.at(1) == channel.name)
			{
				sendMsgToChannel(channel, client.args.at(1), client.args.at(2));
				return ;
			}
		}
		if (checkChannelNameExists(client.args.at(2)) == 1)
			buf(client, 324, client.args.at(1), "");
		else if (checkClientNickExists(client.args.at(1)) == 1)
			sendMsgToUser(client.args.at(1), client.args.at(2));
	}
	return (0);
}

int	Server::user( t_client &client )
{
	if (client.args.size() < 4)
		buf(client, 461, "", "USER");
	else if (client.username.size() != 0 || client.realname.size() != 0 )
		buf(client, 462, "", "");
	else if (client.args.at(2) != "0" && client.args.at(3) != "*")
		return ;
	else
	{
		std::cout << "setting " << client.nickname << "'s user to " << client.args.at(1) << " and real name to " << client.args.at(4) << std::endl;
		client.username = client.args.at(1);

		//must be the last parameter because it may contain SPACE (' ', 0x20) characters, and should be prefixed with a colon (:) if required
		client.realname = client.args.at(4);
		buf(client, 0, client.args.at(1) + " 0 * " + client.args.at(4), "USER");
	}
	return (0);
}

int	Server::cap( t_client &client )
{
	(void) client;
	return (0);
}