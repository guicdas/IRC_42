#include "../includes/irc.hpp"


void	Server::nick( t_client *client, std::string arg ){
	std::string	forbiddenChars[10] = {",*.?!@\"$#"};

	// falta o ':' que so pode estar em primeiro
	if (std::strpbrk(arg.c_str(), forbiddenChars->c_str()) != NULL)
	{
		buf(client, 432, "");
		return ;
	}
	if (std::strcmp(client->nickname.c_str(), arg.c_str()) == 0)
		return ;
	std::cout << "changing client's name from " << client->nickname << " to " << arg <<  std::endl;
	buf(client, 0, arg);
	client->nickname = arg;
}

void	Server::join( t_client *client, std::string arg ){
	std::string	forbiddenChars[4] = {",\x07 "};

	if (arg[0] != '#' && arg[0] != '&')
		buf(client, 403, arg);
	else if (std::strpbrk(arg.c_str(), forbiddenChars->c_str()) != NULL)
		buf(client, 403, arg);
	else
	{
		for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
		{
			t_channel channel = *itCh;
			if (channel.name == arg)
				return ;
		}
		std::cout << "creating channel " << arg << ENDL;
		t_channel ch;
		ch.name = arg;
		ch.operators.push_back(*client);
		ch.clients.push_back(*client);
		this->channels.push_back(ch);

		std::cout << "client " << client->nickname << " joined " << arg << std::endl;
		buf(client, 0, "!~" + client->nickname + " JOIN " + arg + "* :realname\n");
        Server::mode(client, arg);
		/*
        MODE #canalasd
        << WHO #canalasd %chtsunfra,152
        */
	}
}

void	Server::list( t_client *client, std::string arg ){
	(void) arg;
	std::cout << "client " << client->nickname << " listing all channels: " << std::endl;

	for (std::vector< t_channel >::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		t_channel &ch = *it;
		client->buffer = ch.name + "\t\t" + ch.topic + ".\n";
	}
}

void	Server::privmsg( t_client *client, std::string arg ){
	int found = 0;

	if (arg.size() == 0)
	{
		buf(client, 411, "");
		return ;
	}
	for (std::vector< t_client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		t_client c = *itC;
		if (c.nickname == arg)
		{
			found++;
			break ;
		}
	}
	if (found == 0)
		buf(client, 401, "");
	else if (client->args.size() < 3)
		buf(client, 412, "");
}

void	Server::quit( t_client *client, std::string arg ){
	(void) arg;

	close(client->fd);
	FD_CLR(client->fd, &this->fdList);
	// this->clients.erase(*client);
	std::cout << "client " << client->nickname << " gone" << std::endl;
}

void	Server::mode( t_client *client, std::string arg ){
	int	found = 0;

	for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		t_channel channel = *itCh;
		if (channel.name == arg)
		{
			found++;
			break ;
		}
	}
	if (found == 0)
	{
		buf(client, 403, arg);
		return ;
	}
	buf(client, 324, arg); //If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned.
	// buf(client, 329, ); Servers MAY also return the RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.
	/* Servers MAY choose to hide sensitive information such as channel keys when sending the current modes. */
}