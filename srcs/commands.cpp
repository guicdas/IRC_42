#include "../includes/irc.hpp"


void	Server::nick( t_client *client, std::vector< std::string > args ){
	std::string	forbiddenChars[10] = {",*.?!@\"$#"};

	// falta o ':' que so pode estar em primeiro
	if (std::strpbrk(args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
	{
		buf(client, 432, "", "");
		return ;
	}
	if (std::strcmp(client->nickname.c_str(), args.at(1).c_str()) == 0)
		return ;
	std::cout << "changing client's name from " << client->nickname << " to " << args.at(1) <<  std::endl;
	buf(client, 0, args.at(1), "NICK");
	client->nickname = args.at(1);
}

void	Server::join( t_client *client, std::vector< std::string > args ){
	std::string	forbiddenChars[4] = {",\x07 "};

	if (args.at(1)[0] != '#' && args.at(1)[0] != '&')
		buf(client, 403, args.at(1), "");
	else if (std::strpbrk(args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		buf(client, 403, args.at(1), "");
	else
	{
		if (checkChannelNameExists(args.at(1)) == 1)
		{
			if (isClientInChannel(client, getChannel(args.at(1))))
				return ;
			addUserToChannel(client, getChannel(args.at(1)));
		}
		else
		{
			std::cout << "creating channel " << args.at(1) << ENDL;
			t_channel ch;
			ch.name = args.at(1);
			ch.operators.push_back(*client);
			ch.clients.push_back(*client);
			this->channels.push_back(ch);
		}

		std::cout << "client " << client->nickname << " joined " << args.at(1) << std::endl;
		buf(client, 0, args.at(1) + "* :realname\n", "JOIN");
        //Server::mode(client, args);
		/*
        MODE #canalasd
        << WHO #canalasd %chtsunfra,152
        */
	}
}

void	Server::list( t_client *client, std::vector< std::string > args ){
	(void) args;
	std::cout << "client " << client->nickname << " listing all channels: " << std::endl;

	for (std::vector< t_channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		t_channel &ch = *it;
		client->buffer = ch.name + "\t\t" + ch.topic + ".\n";
	}
}

void	Server::quit( t_client *client, std::vector< std::string > args ){
	(void) args;

	close(client->fd);
	FD_CLR(client->fd, &this->fdList);
	// this->clients.erase(*client);
	std::cout << "client " << client->nickname << " gone" << std::endl;
}

void	Server::mode( t_client *client, std::vector< std::string > args ){
	if (checkChannelNameExists(args.at(1)) == 1)
	{
		buf(client, 403, args.at(1), "");
		return ;
	}
	buf(client, 324, args.at(1), ""); 
	//If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned.
	// buf(client, 329, ); Servers MAY also return the RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.
	/* Servers MAY choose to hide sensitive information such as channel keys when sending the current modes. */
}

void	Server::privmsg( t_client *client, std::vector< std::string > args )
{
	if (client->args.size() < 2)
	{
		buf(client, 411, "", "");
		return ;
	}
	if (client->args.size() < 3)
	{
		buf(client, 412, "", "");
		return ;
	}
	std::cout << "client " << client->nickname << " sending \"" << args.at(1) << "\"" << std::endl;
	
	for (std::vector< t_channel >::iterator itCh = client->channels.begin(); itCh != client->channels.end(); itCh++)
	{
		t_channel &channel = *itCh;
		if (client->args.at(1) == channel.name)
		{
			sendMsgToChannel(&channel, args.at(1), args.at(2));
			return ;
		}
	}

	if (checkChannelNameExists(args.at(2)) == 1)
	{
		buf(client, 324, args.at(1), "");
		return ;
	}
	if (checkClientNickExists(args.at(1)) == 1)
		sendMsgToUser(args.at(1), args.at(2));
}