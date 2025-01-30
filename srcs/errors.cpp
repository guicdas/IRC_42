#include "../includes/irc.hpp"

void	Server::checkChannelNameExists( std::string arg )
{
	for (std::vector< t_channel >::iterator itCh = this->channels.begin(); itCh != this->channels.end(); itCh++)
	{
		t_channel &channel = *itCh;
		if (channel.name == arg)
			throw (403); //client.args.at(1)
	}
}

void	Server::checkClientNickExists( std::string arg )
{
	for (std::vector< t_client >::iterator itC = this->clients.begin(); itC != this->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == arg)
			throw (404); //client.args.at(2)
	}
}

void	checkClientInChannel( t_client &client, t_channel *channel )
{
	for (std::vector< t_client >::iterator itC = channel->clients.begin(); itC != channel->clients.end(); itC++)
	{
		t_client &c = *itC;
		if (c.nickname == client.nickname)
			throw (441); //args 1
	}
}

void	checkClientOp( t_client &c, t_channel *channel )
{
	for (std::vector< t_client >::iterator itC = channel->operators.begin(); itC != channel->operators.end(); itC++)
	{
		t_client &client = *itC;
		if (client.nickname == c.nickname)
			throw (482); //args 1
	}
}